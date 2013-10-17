#include <iostream>
#include <fstream>

#include <state-observation/examples/imu-attitude-trajectory-reconstruction.hpp>

using namespace stateObservation;

DiscreteTimeArray getMeasurements(char * accelerometerSignal,  char * gyrometerSignal)
{
    std::ifstream facc;
    std::ifstream fgyr;

    facc.open(accelerometerSignal);
    fgyr.open(gyrometerSignal);

    Vector3 mAcc;
    Vector3 mGyr;

    Vector yk=Vector::Zero(6,1);

    DiscreteTimeArray y;

    bool continuation=true;

    while (continuation)
    {
        unsigned k1;
        unsigned k2;
        facc >> k1;
        fgyr >> k2;
        if (facc.eof()||facc.eof()||k1!=k2)
            continuation=false;

        if (continuation)
        {
            facc >> mAcc[0]>> mAcc[1]>> mAcc[2];
            fgyr >> mGyr[0]>> mGyr[1]>> mGyr[2];

            yk.head(3)=mAcc;
            yk.tail(3)=mGyr;
            y.pushBack(yk,k1);
        }
    }

    return y;
}

int test(const DiscreteTimeArray & y)
{

    std::cout << "Starting observation" <<std::endl;

    ///sampling period
    const double dt=5e-3;

    ///Sizes of the states for the state, the measurement, and the input vector
    const unsigned stateSize=18;
    const unsigned measurementSize=6;
    const unsigned inputSize=6;

    Vector xh0=Vector::Zero(stateSize,1);

    Matrix p=Matrix::Identity(stateSize,stateSize)*0.1;

    ///The covariance matrix of the process noise and the measurement noise
    Matrix q = Matrix::Identity(stateSize,stateSize)*0.01;
    Matrix r = Matrix::Identity(measurementSize,measurementSize)*100;

    DiscreteTimeArray xh = examples::imuAttitudeTrajectoryReconstruction
                                        (y, xh0, p, q, r, dt);

    ///file of output
    std::ofstream f;
    f.open("trajectory.dat");

    ///the reconstruction of the state
    for (int i=xh.getFirstTime();i<=xh.getLastTime();++i)
    {
        ///display part,
        Vector3 gh;
        Matrix3 Rh;
        {
            Vector3 orientationV=Vector(xh[i]).segment(9,3);
            double angle=orientationV.norm();
            if (angle > cst::epsilonAngle)
                Rh = AngleAxis(angle, orientationV/angle).toRotationMatrix();
            else
                Rh = Matrix3::Identity();
            gh=Rh.transpose()*cst::gravity;
        }

        AngleAxis a(Rh);



        f << i<< " \t "<< a.angle() * 180 / M_PI << " \t\t "
        << a.axis().transpose() << " \t\t " <<  gh.transpose()
        << std::endl;

    }
    return 0;
}

int main()
{

    return test(getMeasurements("dg_HRP2LAAS-accelerometer.dat","dg_HRP2LAAS-gyrometer.dat"));

}