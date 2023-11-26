#include <iostream>
#include <cmath>

class Object {
public:
    virtual ~Object() = default;
    virtual double ModelFunction(double Yt, double Ut) = 0;
    virtual void OutputModel(double y, double u, int NumLin) = 0;
};
class NotModelLiner : public Object
{
public:
    double a;
    double b;
    double prev_ut = 0;
    double prev_yt = 0;
    double c;
    double d;
    NotModelLiner(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {}
    ~NotModelLiner() override = default;
    double ModelFunction(double yt, double ut) override {
        return a * yt - b * pow(prev_yt, 2) + c * ut + d * sin(prev_ut);
    }
    void OutputModel(double y, double u, int NotNumLin) override
    {
        double Yt = y;
        double Ut = u;

        std::cout << std::endl << std::endl << "Not liner model";
        std::cout << "\t\t\tIteration number:" << "\t\t\t" << "Yt";
        for (int i = 0; i < NotNumLin; i++) {
            if (i > 0) {
                prev_ut = Ut;
                prev_yt = Yt;
                std::cout << std::endl << "enter value Ut:";
                std::cin >> Ut;
                Yt = this->ModelFunction(Yt, Ut);
                std::cout << "\t\t\t\t" << i + 1 << "\t\t\t\t" << Yt;
            }
            else {
                prev_ut = 0; prev_yt = 0;
                std::cout << std::endl << "enter value Ut:";
                std::cin >> Ut;

                Yt = this->ModelFunction(Yt, Ut);
                std::cout << "\t\t\t\t" << i + 1 << "\t\t\t\t" << Yt;
            }
        }

    }
};

class ModelLiner : public Object
{
public:
    double a;
    double b;
    ModelLiner(double a, double b) : a(a), b(b) {}
    ~ModelLiner() override = default;
    double ModelFunction(double yt, double ut) override {
        return a * yt + b * ut;
    }
    void OutputModel(double y, double u, int NumLin) override
    {
        double Yt = y;
        double Ut = u;
        std::cout << std::endl << std::endl << "Liner model";
        std::cout << "\t\t\tIteration number:" << "\t\t\t" << "Yt";
        for (int i = 0; i < NumLin; i++) {
            std::cout << std::endl << "enter value Ut:";
            std::cin >> Ut;

            Yt = this->ModelFunction(Yt, Ut);
            std::cout << "\t\t\t\t" << i + 1 << "\t\t\t\t" << Yt;
        }
    }
};

class Regulator
{
private:

    const double _td = 40;
    const double _t0 = 10;
    const double _k = 0.1;
    const double _t = 10;
    const double timeModeling = 30;
    double _uk = 0;

    double CurrentValueUk(double e, double e1, double e2)
    {
        double q0 = _k * (1 + _td / _t0);
        double q1 = -_k * (1 + 2 * _td / _t0 - _t0 / _t);
        double q2 = _k * _td / _t0;
        _uk += q0 * e + q1 * e1 + q2 * e2;
        return _uk;
    }

public:
    void Regulate(double w, double y0, Object& obj)
    {
        double e1 = 0;
        double e2 = 0;
        double y = y0;
        for (int i = 1; i <= timeModeling; i++) {
            double e;
            e = w - y;
            _uk = CurrentValueUk(e, e1, e2);
            y = obj.ModelFunction(y0, _uk);
            std::cout << "E = " << e << ", Yt = " << y << ", Uk = " << _uk << std::endl;
            e2 = e1;
            e1 = e;
        }
        _uk = 0;
    }
};

int main()
{
    double y = 0;
    double u = 0;
    double bLiner = 0;
    double aLiner = 0;
    double bNotLiner = 0;
    double aNotLiner = 0;
    double c = 0;
    double d = 0;
    int NumLin = 0;
    int NumNotLin = 0;
    const double w = 8;
    const double y0 = 3;
    std::cout << "Number of iteration" << std::endl;

    std::cout << "Liner Model:";
    std::cin >> NumLin;

    std::cout << "Not Liner Model:";
    std::cin >> NumNotLin;
    std::cout << "enter value y:";
    std::cin >> y;
    std::cout << "enter value u:";
    std::cin >> u;

    std::cout << std::endl << "Liner model" << std::endl;
    std::cout << "enter value a:";
    std::cin >> aLiner;
    std::cout << "enter value b:";
    std::cin >> bLiner;

    std::cout << std::endl << "Not liner model" << std::endl;
    std::cout << "enter value a:";
    std::cin >> aNotLiner;
    std::cout << "enter value b:";
    std::cin >> bNotLiner;
    std::cout << "enter value c:";
    std::cin >> c;
    std::cout << "enter value d:";
    std::cin >> d;
    ModelLiner modelLin(aLiner, bLiner);
    modelLin.OutputModel(y, u, NumLin);
    NotModelLiner notmodelLin(aNotLiner, bNotLiner, c, d);
    notmodelLin.OutputModel(y, u, NumLin);

    Regulator PIDregulator;


    std::cout << "Liner Model:"<<std::endl;
    PIDregulator.Regulate(w, y0, modelLin);
    std::cout << std::endl;

    std::cout << "Not linear model" << std::endl;
    PIDregulator.Regulate(w, y0, notmodelLin);
    system("Pause");

}