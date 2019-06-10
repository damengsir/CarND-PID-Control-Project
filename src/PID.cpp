#include "PID.h"
#include <limits>
#include <iostream>
using namespace std;

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
	parameters.push_back(Kp_);
	parameters.push_back(Ki_);
	parameters.push_back(Kd_);

	this->p_error = 0.0;
	this->i_error = 0.0;
	this->d_error = 0.0;

	need_twiddle = true;
	step = 1;
	val_step = 100;
	test_step = 1000;

	for (int i = 0; i < 3;i++) {
		//init the change rate with the value of 0.1*parameter
		changes.push_back(0.1*parameters[i]);
	}
	index_param = 0;

	best_error = std::numeric_limits<double>::max();
	total_error = 0;
	//fail to make the total_error better times
	fail_counter = 0;


}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
	if (step ==1) {
		p_error = cte;
	}
	d_error = cte - p_error;
	p_error = cte; //how far we off the center of the road
	i_error += cte ;//the sum of all of our cross check errors
	

	if (need_twiddle) 
	{
		//update total error when we past some steps.
      cout<<"************"<<"step"<<step<<endl;
		if (step % (val_step + test_step) > val_step)
		{
			total_error += (cte * cte);
		}
		if (step % (val_step + test_step) == 0) {
			if (step ==(val_step+test_step)) {
				if (total_error<best_error) {
					best_error = total_error;
				}
				parameters[index_param] += changes[index_param];
			}
			else {
				if (total_error<best_error) {
					best_error = total_error;
					changes[index_param] *= 1.1;
					IndexMove();
					parameters[index_param] += changes[index_param];
					fail_counter = 0;
				}
				else if (fail_counter ==0) {
					parameters[index_param] -= changes[index_param];
					fail_counter++;
				}
				else {
					parameters[index_param] += changes[index_param];
					changes[index_param] *= 0.9;
					IndexMove();
					parameters[index_param] += changes[index_param];
					fail_counter = 0;
				}

			}
        cout<<"LLLLLLLLLLLLLLLLLLLLLLLL"<<endl;
        cout << "P:" << parameters[0] << "I:" << parameters[1] << "D" << parameters[2] << endl;
			cout << "best_error: " << best_error << "total_error: " << total_error << endl;
			cout << "change_index: " << index_param << "new_parameter: " << parameters[index_param] << endl;
			total_error = 0;
        cout<<"LLLLLLLLLLLLLLLLLLLLLLLL"<<endl;

		}
      /*
      cout << "P:" << parameters[0] << "I:" << parameters[1] << "D" << parameters[2] << endl;
		 cout << "best_error: " << best_error << "total_error: " << total_error << endl;
      */
	}
	step++;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return -parameters[0] * p_error - parameters[1] * i_error - parameters[2] * d_error;
}



void PID::IndexMove() {
	index_param++;
	if (index_param >= 3) {
		index_param = 0;
	}
}