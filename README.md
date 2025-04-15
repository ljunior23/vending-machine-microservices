Vending Machine Microservices
A microservices-based beverage recommendation system that suggests drinks based on weather conditions.
Overview
This application consists of four microservices:
Nginx Reverse Proxy: Routes client requests to appropriate services
Weather Service: Determines weather type (WARM/COLD) based on city ID
Beverage Preference Service: Recommends beverages based on weather type
Order Beverage Service: Coordinates between services to fulfill orders
Architecture
Architecture DiagramThe application follows a microservices architecture where:
Client requests are sent to the Nginx reverse proxy
Order Beverage Service receives the request with a city ID
Weather Service determines if it's WARM or COLD based on city ID (odd = WARM, even = COLD)
Beverage Preference Service recommends appropriate drinks based on weather
Response is returned to the client through the Nginx proxy
Installation
Prerequisites
Docker and Docker Compose
Git
Setup
Clone the repository:
bash
git clone https://github.com/ljunior23/vending-machine-microservices.git
cd vending-machine-microservices

Build and run the services:
bash
docker-compose up -d

Verify all services are running:
bash
docker ps

Usage
Testing the Application
Run the provided test script to generate requests:
bash
./script/generate_request.sh > output.txt

API Endpoints
GET /api/beverage?city={cityId}: Get a beverage recommendation for a specific city
Implementation Details
Weather Service
Determines weather type based on city ID
Odd city IDs return WARM weather
Even city IDs return COLD weather
Beverage Preference Service
Recommends hot beverages (cappuccino, latte, espresso) for COLD weather
Recommends cold beverages (lemonade, ice tea, soda) for WARM weather
Order Beverage Service
Coordinates communication between services
Translates weather type to beverage type
Returns final beverage recommendation
Docker Images
The Docker images for this project are available on Docker Hub:
ney24/vending-machine-microservices:v1.0
Development
Modifying the Services
Update the Thrift definition in vending-machine.thrift
Regenerate the service code
Rebuild the Docker images
Building Docker Images
bash
docker build -t ney24/vending-machine-microservices:v1.0 .

Troubleshooting
Common Issues
If Nginx fails to start, check the Lua files in /gen-lua/ for syntax errors
If services can't communicate, verify network settings in docker-compose.yml
License
This project is for educational purposes as part of a university assignment.
Acknowledgments
Original application structure provided by Probir Roy
