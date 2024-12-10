#include "HexaIK.h"

namespace hexapod
{
    KinematicsConfig::KinematicsConfig(double coxa, double femur, double tibia) 
        : _d_coxa(coxa), _d_femur(femur), _d_tibia(tibia)
    {
    }

    HexaIK::HexaIK(const KinematicsConfig& cfg) : config(cfg)
    {
    }

    int HexaIK::radians_to_degrees(double radians)
    {
        return static_cast<int>(std::round(radians * (180.0 / M_PI)));
    }

    HexaIK::ik_ret_e_t HexaIK::compute(double X, double Y, double Z, Angles& angles)
    {
        angles.coxa = radians_to_degrees(atan2(Y, X));

        double trueX = sqrt(X * X + Y * Y) - config._d_coxa;
        double im = sqrt(trueX * trueX + Z * Z);
        double q1 = atan2(Z, trueX);

        double n1 = (config._d_tibia * config._d_tibia) - (config._d_femur * config._d_femur) - (im * im);
        double d2 = -2 * config._d_femur * im;

        if (d2 == 0 || fabs(n1 / d2) > 1) {
            return IK_FAIL;
        }

        double q2 = acos(n1 / d2);
        angles.femur = radians_to_degrees(q2 + q1);

        n1 = (im * im) - (config._d_femur * config._d_femur) - (config._d_tibia * config._d_tibia);
        d2 = -2 * config._d_femur * config._d_tibia;

        if (d2 == 0 || fabs(n1 / d2) > 1) {
            return IK_FAIL;
        }

        angles.tibia = radians_to_degrees(acos(n1 / d2));
        return IK_SUCCESS;
    }

    void HexaIK::real_angle(ik_leg_id leg, Angles& angles) const
    {
        switch (leg) {
            case ik_leg_id::LEFT_FRONT:
                angles.coxa = (angles.coxa >= 0) ? angles.coxa - 45 : 315 - std::abs(angles.coxa);
                break;
            case ik_leg_id::LEFT_MIDDLE:
                angles.coxa = (angles.coxa >= 0) ? angles.coxa - 90 : 270 - std::abs(angles.coxa);
                break;
            case ik_leg_id::LEFT_REAR:
                angles.coxa = (angles.coxa >= 0) ? angles.coxa - 135 : 225 - std::abs(angles.coxa);
                break;
            case ik_leg_id::RIGHT_FRONT:
                angles.coxa = angles.coxa + 45;
                break;
            case ik_leg_id::RIGHT_MIDDLE:
                angles.coxa = angles.coxa + 90;
                break;
            case ik_leg_id::RIGHT_REAR:
                angles.coxa = angles.coxa + 135;
                break;
        }

        angles.femur = 90 - angles.femur;
        angles.tibia = (angles.tibia <= 45) ? 0 : angles.tibia - 45;
    }

    HexaIK::ik_ret_e_t HexaIK::do_1_leg_ik(Angles& angles, ik_leg_id leg, double X, double Y, double Z)
    {
        // Calcula los ángulos
        if (compute(X, Y, Z, angles) != IK_SUCCESS)
        {
            return IK_FAIL;
        }

        // Ajusta los ángulos reales
        real_angle(leg, angles);

        return IK_SUCCESS;
    }

} // namespace hexapod
