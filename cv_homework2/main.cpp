//****************************//
//Author:Song Jin
//Last modify time:2017/9/29
//version:1.0
//License:MIT
//Email:15754603603@163.com
//****************************//
#include <iostream> // for standard I/O
#include <vector>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
MatrixXd errorF(Vector2d &x, int N, vector<double> &x_data, vector<double> &y_data)
{
	MatrixXd error(N, 1);
	for (int i = 0; i < N; i++)
	{
		error(i, 0) = y_data[i] - x(0) * cos(x(1) * x_data[i]) - x(1) * sin(x(0) * x_data[i]);
	}
	return error;
}
MatrixXd jacobian(Vector2d &x, int N, vector<double> &x_data, vector<double> &y_data) 
{
	MatrixXd jaco(N, 2);
	for (int i = 0; i < N; i++)
	{
		jaco(i, 0) = cos(x(1) * x_data[i]) + x(1) * x_data[i] * cos(x(0) * x_data[i]);
		jaco(i, 1) = -x(0) * x_data[i] * sin(x(1) * x_data[i]) + sin(x(0) * x_data[i]);
	}
	return jaco;
}
int main()
{
	vector<double> x_data = { 0,0.1000,0.2000,0.3000,0.4000,0.5000,0.6000,0.7000,
		0.8000,0.9000,1.0000,1.1000,1.2000,1.3000,1.4000,1.5000,
		1.6000,1.7000,1.8000,1.9000,2.0000,2.1000,2.2000,2.3000,
		2.4000,2.5000,2.6000,2.7000,2.8000,2.9000,3.0000,3.1000,
		3.2000,3.3000,3.4000,3.5000,3.6000,3.7000,3.8000,3.9000,
		4.0000,4.1000,4.2000,4.3000,4.4000,4.5000,4.6000,4.7000,
		4.8000,4.9000,5.0000,5.1000,5.2000,5.3000,5.4000,5.5000,
		5.6000,5.7000,5.8000,5.9000,6.0000,6.1000,6.2000 };
	vector<double> y_data = { 84.8813,146.7663,149.6254,93.1977,3.6583,-104.9392,
		-177.0434,-150.9106,-60.8202,48.5765,154.1925,206.0641,151.7364,63.3721,
		-55.5942,-158.1013,-164.6986,-109.7675,0.1444,90.0419,141.2188,146.5047,
		81.6069,-14.3515,-70.5310,-96.3240,-59.9917,-5.2409,52.9212,62.2246,
		56.0940,0.4309,-25.9245,-35.8837,-6.8627,44.8732,91.0407,82.4212,56.4174,
		-13.5477,-101.5125,-114.6539,-84.9267,2.7138,87.8119,173.4679,175.2198,
		99.4924,-9.1194,-109.7865,-168.8934,-164.3817,-66.9185,50.9492,167.1449,
		192.5162,161.2291,48.3567,-53.2931,-44.5062,-142.7662,-99.8187,6.7414 };
	int N = x_data.size();
	MatrixXd J, F, F_next;
	Vector2d delta_x, x_next, x;
	double error_min = 100000.0;
	Vector2d optimal_x;
	int count = 0;
	// set a b [50,150] step_length = 1
	for (double a0 = 50; a0 < 150; a0 += 1)
	{
		for (double b0 = 50; b0 < 150; b0 += 1)
		{
			x = { a0, b0 };
			double lambda = 10;
			int iteration_N = 0;
			delta_x = { 1,1 };
			while (delta_x.norm() > 0.01 && iteration_N < 50)
			{
				J = jacobian(x, N, x_data, y_data);
				F = errorF(x, N, x_data, y_data);
				delta_x = (J.transpose() * J + lambda * MatrixXd::Identity(2, 2)).inverse()*(-J.transpose()*F);
				x_next = x + delta_x;
				F_next = errorF(x_next, N, x_data, y_data);
				double p = (F_next - F).norm() / (J * delta_x).norm();
				if (p > 0.85)
				{
					x = x_next;
					cout << "dx " << delta_x.norm() << endl;
				}
				if (p > 0.75)
				{
					lambda *= 0.5;
					cout << "less " << p <<" lambda " << lambda << endl;
				}
				else if(p < 0.25)
				{
					lambda *= 2;
					cout << "big " << p << " lambda " << lambda << endl;
				}
				iteration_N++;
			}
			if (F.norm() < error_min)
			{
				optimal_x = x;
				error_min = F.norm();
			}
			cout << count++ << " error_min = " << error_min
				<< " optimal_a = " << optimal_x[0]
				<< " optimal_b = " << optimal_x[1] << endl;
		}
	}
	return 0;	
}