#ifndef HEXAPOD_INVERSE_KINEMATICS_H
#define HEXAPOD_INVERSE_KINEMATICS_H

#include <cmath>
#include <cstdint>

namespace hexapod
{
    enum class ik_leg_id
    {
        LEFT_FRONT,
        LEFT_MIDDLE,
        LEFT_REAR,
        RIGHT_FRONT,
        RIGHT_MIDDLE,
        RIGHT_REAR 
    };

    struct KinematicsConfig
    {
        double _d_coxa;
        double _d_femur;
        double _d_tibia;

        KinematicsConfig(double coxa, double femur, double tibia);
    };

    class HexaIK
    {
        public:
            struct Angles
            {
                int coxa;
                int femur;
                int tibia;
            };

            typedef enum ik_ret_e
            {
                IK_FAIL = 0,
                IK_SUCCESS
            }ik_ret_e_t;

        private:
            const KinematicsConfig& config;
            static int radians_to_degrees(double radians);

        public:
            explicit HexaIK(const KinematicsConfig& cfg);

            ik_ret_e_t compute(double X, double Y, double Z, Angles& angles);
            void real_angle(ik_leg_id leg, Angles& angles) const;
            ik_ret_e_t do_1_leg_ik(Angles& angles, ik_leg_id leg, double X, double Y, double Z);
    };

} // namespace hexapod

#endif // HEXAPOD_INVERSE_KINEMATICS_H
