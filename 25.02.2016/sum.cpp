#include <iostream>
#include <fstream> 
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <mutex>
const int N = 1e8;
std::vector<double> data(N);

void fill(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 2);
	for(auto &i : data){
		i = dis(gen);
	}
}
std::mutex mtx;
void sum(int l, int r, double &answer){
	double temp = 0;
	for(size_t i = l; i < r; ++i){
		temp += data[i];
	}
	mtx.lock();
	answer += temp;
	mtx.unlock();
	
}
double compute(int num, double &result){
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	std::vector<std::thread> threads(num);
	const int chunk_size = N / num;
	double vector_sum = 0;
	int current_boarder = 0;
	for(auto it = std::begin(threads); it != std::end(threads) ; ++it){	
		*it = std::thread(sum, current_boarder, it == std::end(threads) - 1 ? N : current_boarder + chunk_size, std::ref(vector_sum));
		current_boarder += chunk_size;
	}
	for(auto&& i : threads) {
   		i.join();
  	}
	end = std::chrono::system_clock::now();
 	std::chrono::duration<double> elapsed_seconds = end-start;
	result = vector_sum;	 
	return elapsed_seconds.count();
}


double stupid(){
	double s = 0;
	for(auto &i : data) {
		s += i;
	}
	return s;
}
const double EPS = 1e-6;
int main(){
	fill();
	std::ofstream myfile;
	myfile.open ("data.txt");
	double ans = 0, stupid_ans = 0;
	//stupid_ans = stupid();
	for(int i = 1; i <= 20; ++i){
		myfile << i << " " << compute(i, ans) << std::endl;
	}
	return 0;
}
