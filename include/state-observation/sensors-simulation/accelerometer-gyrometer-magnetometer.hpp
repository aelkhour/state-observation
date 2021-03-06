/**
 * \file      accelerometer-gyrometer-magnetometer.hpp
 * \author    Mehdi Benallegue, Joseph Mirabel
 * \date      2015
 * \brief     Implements the accelerometer-gyrometer-magnetometer inertial
 *            measurement unit
 *
 *
 */



#ifndef SIMULATIONACCELEROMETERGYROMETERMAGNETOMETERSENSORHPP
#define SIMULATIONACCELEROMETERGYROMETERMAGNETOMETERSENSORHPP

#include <Eigen/Core>
#include <boost/assert.hpp>

#include <state-observation/sensors-simulation/algebraic-sensor.hpp>

#include <state-observation/sensors-simulation/algorithm/linear-acceleration.hpp>

#include <state-observation/sensors-simulation/algorithm/rotation-velocity.hpp>

#include <state-observation/sensors-simulation/algorithm/magnetic-field.hpp>

namespace stateObservation
{
    /**
     * \class  AccelerometerGyrometerMagnetometer
     * \brief  Implements the accelerometer-gyrometer-magnetometer measurements
     *
     *
     *
     * \details
     *
     */

    class AccelerometerGyrometerMagnetometer : public AlgebraicSensor,
        protected algorithm::LinearAcceleration,
        protected algorithm::RotationVelocity,
        protected algorithm::MagneticField
    {
    public:
        AccelerometerGyrometerMagnetometer();

        ///Virtual destructor
        virtual ~AccelerometerGyrometerMagnetometer(){}



        void setMatrixMode(bool matrixMode);


    protected:
        ///Gets the state vector Size
        virtual unsigned getStateSize_() const;

        ///Gets the measurements vector size
        virtual unsigned getMeasurementSize_() const;


        virtual Vector computeNoiselessMeasurement_();

        Matrix3 r_;
        Vector3 acc_;
        Vector3 omega_;
        Vector3 magne_;
        Vector output_;

        bool matrixMode_;

        static const int stateSize_= 10;
        static const int stateSizeMatrix_= 15;

        static const int measurementSize_=9;

        int currentStateSize_;

    };

}

#endif // SIMULATIONACCELEROMETERGYROMETERMAGNETOMETERSENSORHPP
