#include "mnx/assembly/AssemblySolver.h"

#include "mnx/core/math/Tolerance.h"

#include <cmath>

namespace mnx {

namespace {

[[nodiscard]] Vec3 translationOf(const Transform3D& transform) {
    return {transform.matrix()(0, 3), transform.matrix()(1, 3), transform.matrix()(2, 3)};
}

[[nodiscard]] Transform3D withTranslation(const Transform3D& transform, const Vec3& translation) {
    Mat4 matrix = transform.matrix();
    matrix(0, 3) = translation.x();
    matrix(1, 3) = translation.y();
    matrix(2, 3) = translation.z();
    return Transform3D(matrix);
}

[[nodiscard]] Transform3D translated(const Transform3D& transform, const Vec3& delta) {
    return withTranslation(transform, translationOf(transform) + delta);
}

[[nodiscard]] Status requireWorldTransforms(
    const AssemblyDocument& assembly,
    const MateConstraint& mate,
    Transform3D& first,
    Transform3D& second
) {
    Result<Transform3D> firstWorld = assembly.worldTransform(mate.firstComponentId());
    if(!firstWorld) {
        return firstWorld.status();
    }
    Result<Transform3D> secondWorld = assembly.worldTransform(mate.secondComponentId());
    if(!secondWorld) {
        return secondWorld.status();
    }
    first = firstWorld.value();
    second = secondWorld.value();
    return Status::ok();
}

[[nodiscard]] double angleInXY(const Vec3& direction) {
    return std::atan2(direction.y(), direction.x());
}

} // namespace

AssemblySolveResult AssemblySolver::solve(AssemblyDocument& assembly) const {
    AssemblySolveResult result {Status::ok(), {}, {}};
    for(const MateConstraint& mate : assembly.mates()) {
        Status status = applyMate(assembly, mate);
        if(!status) {
            result.status = status;
            result.diagnostics.push_back(status.message());
            return result;
        }
        result.appliedMateIds.push_back(mate.id());
    }
    result.diagnostics.push_back("Assembly mates applied deterministically");
    return result;
}

Status AssemblySolver::applyMate(AssemblyDocument& assembly, const MateConstraint& mate) const {
    if(mate.type() == MateType::Fixed) {
        return assembly.setWorldTransform(mate.firstComponentId(), mate.fixedTransform());
    }

    Transform3D first;
    Transform3D second;
    Status transformStatus = requireWorldTransforms(assembly, mate, first, second);
    if(!transformStatus) {
        return transformStatus;
    }

    if(mate.type() == MateType::Coincident) {
        const Vec3 firstPoint = first.applyToPoint(mate.firstLocalPoint());
        const Vec3 secondPoint = second.applyToPoint(mate.secondLocalPoint());
        return assembly.setWorldTransform(mate.secondComponentId(), translated(second, firstPoint - secondPoint));
    }

    if(mate.type() == MateType::Distance) {
        const Vec3 firstPoint = first.applyToPoint(mate.firstLocalPoint());
        const Vec3 secondPoint = second.applyToPoint(mate.secondLocalPoint());
        Vec3 direction = secondPoint - firstPoint;
        if(isZero(direction.norm())) {
            direction = {1.0, 0.0, 0.0};
        } else {
            direction = direction.normalized();
        }
        const Vec3 targetPoint = firstPoint + direction * mate.distance();
        return assembly.setWorldTransform(mate.secondComponentId(), translated(second, targetPoint - secondPoint));
    }

    if(mate.type() == MateType::Parallel) {
        const Vec3 firstDirection = first.applyToVector(mate.firstLocalDirection()).normalized();
        const Vec3 secondDirection = second.applyToVector(mate.secondLocalDirection()).normalized();
        if(isZero(firstDirection.norm()) || isZero(secondDirection.norm())) {
            return Status::invalidArgument("Parallel mate directions must be non-zero");
        }
        const double delta = angleInXY(firstDirection) - angleInXY(secondDirection);
        const Vec3 currentTranslation = translationOf(second);
        const Transform3D rotated = Transform3D::rotationZ(delta) * second;
        return assembly.setWorldTransform(mate.secondComponentId(), withTranslation(rotated, currentTranslation));
    }

    return Status::invalidArgument("Unsupported assembly mate type");
}

} // namespace mnx
