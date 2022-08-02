#pragma once

#include "NodeInterfaceAStar.h"

#include <vector>
#include <chrono>
#include <iostream>
#include <list>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <unordered_set>
#include <unordered_map>
#include <map>

class AStarSearch
{
	std::multimap<float, NodeInterfaceAStar*> openMap;


	std::unordered_map<int, float> hashes;
	bool alternate = true;

	NodeInterfaceAStar* currentNode;
	NodeInterfaceAStar* currentRoot;
	NodeInterfaceAStar* endNode;

	mutable std::shared_mutex openMut;

	std::mutex hashMut;
	std::mutex hashAccessLock;

	int numThreads;




	long long timeAdd = 0;
	long long timeExpand = 0;
	long long timeAddHash = 0;
	long long timeAddOpen = 0;
	long long timeRepeated = 0;
	long long timeAddClosed = 0;

	int bestEval = 10000;
	int countRep = 0;
	int counter = 0;

	bool end = false;
	int currentDepth;



public:

	AStarSearch(NodeInterfaceAStar* origin, NodeInterfaceAStar* end, int threads)
	{
		addOpen(origin, false);

		this->currentNode = origin;
		this->currentRoot = origin;
		this->endNode = end;
		this->numThreads = threads;
		this->currentDepth = 0;

		hashes.reserve(100000);
		


	}


	NodeInterfaceAStar* execute(int remainingTimeMillis)
	{

		long long timeTotal = 0;
		bool threaded = false;




		auto time = std::chrono::steady_clock::now();





		if (this->numThreads > 1)
		{
			threaded = true;
			std::thread first(&AStarSearch::run, this, threaded, remainingTimeMillis);
			std::this_thread::sleep_for(std::chrono::milliseconds(2));

			if (this->numThreads == 4)
			{

				std::thread second(&AStarSearch::run, this, threaded, remainingTimeMillis);
				std::thread third(&AStarSearch::run, this, threaded, remainingTimeMillis);
				std::thread fourth(&AStarSearch::run, this, threaded, remainingTimeMillis);

				second.join();
				third.join();
				fourth.join();

			}
			first.join();

		}
		else
		{
			this->run(threaded, remainingTimeMillis);
		}
			
	

		timeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();



		timeExpand /= 1000000;
		timeAddOpen /= 1000000;
		timeAddClosed /= 1000000;
		timeRepeated /= 1000000;
		timeTotal /= 1000000;

		std::cerr << "\n\nFinished:\n" << "Total time: " << timeTotal << "  Counter: " << counter << " Repetition Counter: " << countRep << "  Total Moves until finish: " << currentNode->getDepth() << "\nTime Expanding: " << timeExpand << "\nTime checking repeated: " << timeRepeated << "\nTime Adding to Open: " << timeAddOpen << "\nTime Adding to Closed: " << timeAddClosed << std::endl;

		timeAddHash /= 1000000;
		timeAdd /= 1000000;

		std::cerr << "Time adding to hashing vector: " << timeAddHash << "\nTime Inserting in list: " << timeAdd << std::endl;
		std::cerr << "Size open: " << this->openMap.size() << " Size hashes: " << this->hashes.size() << std::endl;

		this->end = false;

		


		//returning proper depth node
		NodeInterfaceAStar* node = this->currentNode;
		while (node->getDepth() > this->currentDepth+1)
		{
			node = node->getParent();
		}
		this->currentDepth++;

		this->currentRoot = node;

		return node;


	}








	void run(bool threaded, int remainingTimeMillis)
	{

		auto startTime = std::chrono::steady_clock::now();
		auto endTime = startTime + std::chrono::milliseconds(remainingTimeMillis);
		auto time = startTime;




		bool repeatedNodes[20];
		std::vector<NodeInterfaceAStar*> expandedNodes;

		NodeInterfaceAStar* threadedNode = this->currentNode;

		while (!threadedNode->equalsState(this->endNode) && end == false && time < endTime)
		{

			time = std::chrono::steady_clock::now();
			//if(threadedNode != nullptr && threadedNode != this->currentNode)
				//delete threadedNode;

			threadedNode = getOpenFront(threaded);
			counter++;

			timeAddClosed += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();



			time = std::chrono::steady_clock::now();

			expandedNodes = threadedNode->expand();

			timeExpand += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();



			time = std::chrono::steady_clock::now();
			for (int i = 0; i < expandedNodes.size(); i++)
			{
				expandedNodes[i]->evaluate();
				expandedNodes[i]->calculateHash();
				//check if repeated
				repeatedNodes[i] = this->repeated(expandedNodes[i], threaded);
				countRep++;
			}
			timeRepeated += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();



			time = std::chrono::steady_clock::now();
			for (int i = 0; i < expandedNodes.size(); i++)
			{
				if (!repeatedNodes[i])
				{
					this->addOpen(expandedNodes[i], threaded);
				}
			}
			timeAddOpen += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();





			if (openMap.size() == 0)
			{
				std::cerr << "Exiting due to running out of nodes" << std::endl;
				break;
			}




			if (threadedNode->getEvaluation() < bestEval)
			{
				bestEval = threadedNode->getEvaluation();
				std::cerr << "Best eval updated: " << bestEval << "   Counter: " << counter << std::endl;
				//currentNode->writeNode();

			}




			if (openMap.size() > 1000000)
			{
				std::unique_lock<std::shared_mutex> openLockUnique(openMut);
				if (openMap.size() > 1000000)
				{

					auto mid = openMap.begin();
					std::advance(mid, openMap.size() / 2);

					std::cerr << "Deleting: " << std::distance(mid, openMap.end()) << " Nodes" << std::endl;
					auto aux = mid;
					for (aux; aux != openMap.end(); aux++)
					{
						delete aux->second;
					}


					openMap.erase(mid, openMap.end());

					std::cerr << "Finished Deleting" << std::endl;
				}

				openLockUnique.unlock();
			}



		}

		if (end == false)
		{
			this->currentNode = threadedNode;
			end = true;



		}

	}


	void runTimed(int time)
	{

	}







	bool repeated(NodeInterfaceAStar* a, bool threaded)
	{

		std::pair<std::unordered_map<int, float>::iterator, bool> ret;

		if (threaded)
		{
			hashMut.lock();

			ret = hashes.insert({ a->getHash(), a->getQuality() });

			hashMut.unlock();

			if (!ret.second)
			{

				if ((*ret.first).second > a->getQuality())
				{
					//std::cout << "Old hash quality: " << (*ret.first).second << "  New hash quality:" << a->getQuality() << std::endl;
					hashMut.lock();
					hashes.insert_or_assign(a->getHash(), a->getQuality());
					hashMut.unlock();
					return false;
				}
				delete a;
			}
		}
		else
		{

			ret = hashes.insert({ a->getHash(), a->getQuality() });
			if (!ret.second)
			{

				if ((*ret.first).second > a->getQuality())
				{
					hashes.insert_or_assign(a->getHash(), a->getQuality());
					return false;
				}
				delete a;
			}
		}






		return !ret.second;

	}

	void addOpen(NodeInterfaceAStar* n, bool threaded)
	{
		if (threaded)
		{
			std::vector<NodeInterfaceAStar*>::iterator it;


			// Binary search so that addition to the list is ordered by quality
			auto time = std::chrono::steady_clock::now();
			std::unique_lock<std::shared_mutex> openLockUnique(openMut);

			//it = binarySearch(open.begin(), open.end(), n, 1);
			//open.insert(it, n);
			openMap.insert(std::pair<int, NodeInterfaceAStar*>(n->getQuality(), n));

			openLockUnique.unlock();
			timeAdd += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();
		}
		else
		{

			std::vector<NodeInterfaceAStar*>::iterator it;


			// Binary search so that addition to the list is ordered by quality

			auto time = std::chrono::steady_clock::now();

			//it = binarySearch(open.begin(), open.end(), n, 1);
			//open.insert(it, n);
			openMap.insert(std::pair<int, NodeInterfaceAStar*>(n->getQuality(), n));

			timeAdd += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time).count();


		}


	}

	NodeInterfaceAStar* getOpenFront(bool threaded)
	{
		if (threaded)
		{
			std::unique_lock<std::shared_mutex> openLockUnique(openMut);

			std::multimap<float, NodeInterfaceAStar*>::iterator it = openMap.begin();
			NodeInterfaceAStar* n = (*it).second;
			addClosed(n, threaded);
			return n;
		}
		else
		{
			std::multimap<float, NodeInterfaceAStar*>::iterator it;
			NodeInterfaceAStar* n;
			NodeInterfaceAStar* p;

			do
			{
				it = openMap.begin();
				n = (*it).second;
				addClosed(n, threaded);
				p = n;

				for (int i = 0; i < n->getDepth() - this->currentDepth; i++)
				{
					p = p->getParent();
				}

			} while (p != this->currentRoot);





			return n;
		}


	}

	void addClosed(NodeInterfaceAStar* n, bool threaded)
	{
		//open.erase(open.end()-1);
		openMap.erase(openMap.begin());
	}



	void writePath(NodeInterfaceAStar* n)
	{

		std::list<NodeInterfaceAStar*> path;

		std::cout << "******Writting path*******\n" << std::endl;

		while (n->getParent() != nullptr)
		{
			path.push_front(n);
			n = n->getParent();

		}

		std::cout << "******Writting path*******\n" << std::endl;

		for (NodeInterfaceAStar* n : path)
		{
			n->writeNode();
			std::cout << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

	}



};

