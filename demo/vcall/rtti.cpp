#include <iostream>
#include <vector>

class CarAccessory{
	public: 
		virtual void printType(){
			std::cerr << "My type is is Car Accessory \n";
		}	
	private:
		int ID; 
};


class Tire : public CarAccessory{
	public:
		virtual void printType(){
			std::cerr << "Mytype is a Tire \n";
		}	
	private:
		std::string brand;
};


class Seat: public CarAccessory{
	public:
		virtual void printType(){
			std::cerr << "Mytype is a Seat\n";
		}
	private:
	std::string brand; 
};

int main(int argc, char ** argv){
	
	auto t = new Tire();
	auto s = new Seat();
	
	std::vector<CarAccessory*> accessories; 

	accessories.push_back(t);
	accessories.push_back(s);

	for (auto a : accessories){
		a->printType();
	}

	return 0;
}
