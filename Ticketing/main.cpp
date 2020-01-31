#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include "Player.h"

Player player;
int& position = player.position;
std::atomic_int currentTicket;
std::atomic_int nextTicket;
std::mutex mutex;
int maxUpdates;

void physics()
{
	for (int i = 0; i < maxUpdates; i++)
	{
		int ticketNo = nextTicket++;
		while(ticketNo != currentTicket) //wait until its its turn
		{
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		mutex.lock();
		position = -position; //invert position
		std::cout << "PHYSICS UPDATE: ticket: " << ticketNo << ", position = " << position << std::endl;
		mutex.unlock();

		currentTicket++;
	}
}

void input()
{
	for (int i = 0; i < maxUpdates; i++)
	{
		int ticketNo = nextTicket++;
		while (ticketNo != currentTicket) //wait until its its turn
		{
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		mutex.lock();
		position += rand() % 5 + 1; //add to position
		std::cout << "INPUT UPDATE: ticket: " << ticketNo << ", position = " << position << std::endl;
		mutex.unlock();

		currentTicket++;
	}
}

void render()
{
	for (int i = 0; i < maxUpdates; i++)
	{
		int ticketNo = nextTicket++;
		while (ticketNo != currentTicket) //wait until its its turn
		{
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		mutex.lock();
		//render doesn't update position
		std::cout << "RENDER UPDATE: ticket: " << ticketNo << ", position = " << position << std::endl;
		mutex.unlock();

		currentTicket++;
	}
}

void collision()
{
	for (int i = 0; i < maxUpdates; i++)
	{
		int ticketNo = nextTicket++;
		while (ticketNo != currentTicket) //wait until its its turn
		{
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		mutex.lock();
		position -= rand() % 2 + 1; //decrease position
		std::cout << "COLLISION UPDATE: ticket: " << ticketNo << ", position = " << position << std::endl;
		mutex.unlock();

		currentTicket++;
	}
}

int main()
{
	srand(time(NULL));

	currentTicket = 0;
	nextTicket = 0;
	position = 20;
	maxUpdates = 50;

	std::thread physicsThread = std::thread(physics);
	std::thread renderThread = std::thread(render);
	std::thread collisionThread = std::thread(collision);
	std::thread inputThread = std::thread(input);

	physicsThread.join();
	inputThread.join();
	renderThread.join();
	collisionThread.join();


	return 0;
}