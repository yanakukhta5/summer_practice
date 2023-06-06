#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;

// Структура для хранения данных эксперимента
struct DataPoint {
    double x;
    double y;
};

// Функция для вычисления линии регрессии (метод наименьших квадратов)
void calculateRegressionLine(const vector<DataPoint>& data, double& slope, double& intercept) {
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;

    for (const DataPoint& point : data) {
        double x = point.x;
        double y = point.y;

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    int n = data.size();

    double denominator = n * sumX2 - sumX * sumX;

    // Проверка деления на ноль
    if (denominator == 0) {
        slope = 0.0;
        intercept = 0.0;
        return;
    }

    slope = (n * sumXY - sumX * sumY) / denominator;
    intercept = (sumY - slope * sumX) / n;
}

// Функция для вычисления коэффициента корреляции
double calculateCorrelationCoefficient(const vector<DataPoint>& data) {
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0, sumY2 = 0.0;

    for (const DataPoint& point : data) {
        double x = point.x;
        double y = point.y;

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
        sumY2 += y * y;
    }

    int n = data.size();

    double numerator = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

    // Проверка деления на ноль
    if (denominator == 0) {
        return 0.0;
    }

    return numerator / denominator;
}

// Функция для подбора функциональной зависимости заданного вида (y = Ax^3 + D)
void fitFunction(const vector<DataPoint>& data, double& A, double& D) {
    double sumX = 0.0, sumY = 0.0, sumX2 = 0.0, sumX3 = 0.0, sumX2Y = 0.0;

    for (const DataPoint& point : data) {
        double x = point.x;
        double y = point.y;
        double x2 = x * x;
        double x3 = x * x * x;

        sumX += x;
        sumY += y;
        sumX2 += x2;
        sumX3 += x3;
        sumX2Y += x2 * y;
    }

    int n = data.size();

    double denominator = n * sumX2 * sumX2 * sumX2 - sumX * sumX2 * sumX2 * sumX + n * sumX * sumX * sumX3 - sumX * sumX * sumX2 * sumX;

    // Проверка деления на ноль
    if (denominator == 0) {
        A = 0.0;
        D = 0.0;
        return;
    }

    A = (n * sumX2Y * sumX2 * sumX2 - sumX2 * sumX2 * sumX2Y * sumX + sumX * sumX * sumX2 * sumY - sumX * sumX * sumX3 * sumY) / denominator;
    D = (n * sumX * sumX2Y - sumX2 * sumX2Y + sumX * sumX3 * sumY - sumX * sumX2 * sumY) / denominator;
}

// Функция для вычисления суммарной квадратной ошибки линии регрессии
double calculateSumSquaredError(const vector<DataPoint>& data, double slope, double intercept) {
    double sumError = 0.0;

    for (const DataPoint& point : data) {
        double x = point.x;
        double y = point.y;

        double predictedY = slope * x + intercept;
        double error = y - predictedY;
        sumError += error * error;
    }

    return sumError;
}

// Функция для вычисления средней ошибки линии регрессии
double calculateMeanError(const vector<DataPoint>& data, double slope, double intercept) {
    int n = data.size();
    double sumError = calculateSumSquaredError(data, slope, intercept);
    double meanError = sumError / n;

    return meanError;
}

// Функция для построения графика
void plotGraph(const vector<DataPoint>& data, double A, double D, double slope, double intercept) {
    ofstream dataFile("data.csv");
    ofstream regressionLineFile("regression_line.csv");
    ofstream fittedFunctionFile("fitted_function.csv");

    // Запись данных эксперимента в файл
    for (const DataPoint& point : data) {
        dataFile << point.x << "," << point.y << "\n";
    }

    // Запись точек линии регрессии в файл
    for (const DataPoint& point : data) {
        double y = slope * point.x + intercept;
        regressionLineFile << point.x << "," << y << "\n";
    }

    // Запись точек подобранной функции в файл
    for (const DataPoint& point : data) {
        double y = A * pow(point.x, 3) + D;
        fittedFunctionFile << point.x << "," << y << "\n";
    }

    dataFile.close();
    regressionLineFile.close();
    fittedFunctionFile.close();

    double sumSquaredError = calculateSumSquaredError(data, slope, intercept);
    double meanError = calculateMeanError(data, slope, intercept);
    double correlationCoefficient = calculateCorrelationCoefficient(data);

    cout << "Уравнение регрессии: y = " << slope << "x + " << intercept << endl;
    cout << "Коэффициент корреляции: " << correlationCoefficient << endl;
    cout << "Суммарная квадратичная ошибка (SSE) для линии регрессии: " << sumSquaredError << endl;
    cout << "Средняя ошибка (MAE) для линии регрессии: " << meanError << endl;
    cout << "Корни системы: A = " << A << ", D = " << D << endl;

    vector<DataPoint> fittedData;

    for (const DataPoint& point : data) {
        double y = A * pow(point.x, 3) + D;
        fittedData.push_back({point.x, y});
    }

    double fittedSumSquaredError = calculateSumSquaredError(fittedData, 0, 0);
    double fittedMeanError = calculateMeanError(fittedData, 0, 0);

    cout << "Подобранная функциональная зависимость: y = " << A << "x^3 + " << D << endl;
    cout << "Суммарная квадратичная ошибка (SSE) для подобранной функциональной зависимости: " << fittedSumSquaredError << endl;
    cout << "Средняя ошибка (MAE) для подобранной функциональной зависимости: " << fittedMeanError << endl;
}

int main() {
    setlocale(LC_ALL, "");
    vector<DataPoint> data = {
        {0.0, 0.072},
        {0.2, 0.073},
        {0.4, 0.075},
        {0.6, 0.096},
        {0.8, 0.12},
        {1.0, 0.16},
        {1.2, 0.24},
        {1.4, 0.35},
        {1.6, 0.42},
        {1.8, 0.47}
};

    double slope, intercept;
    calculateRegressionLine(data, slope, intercept);

    double A, D;
    fitFunction(data, A, D);

    plotGraph(data, A, D, slope, intercept);

    return 0;
}
