#include<iostream>
#include<fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <map>
#include <list>
#include <queue>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <iterator>
#include <chrono>
#include <unordered_map>

int main(int argc, char** argv) {
	if (argc == 1) exit(-1);
	bool checkOptimism = false;
	bool checkConsistent = false;
	int algorithm = 0; // 1 - BFS, 2 - UCS, 3- ASTAR
	std::string path;
	std::string heuPath;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "--alg") {
			if (std::string(argv[i + 1]) == "bfs") {
				algorithm = 1;
			}
			else if (std::string(argv[i + 1]) == "ucs") {
				algorithm = 2;
			}
			else if (std::string(argv[i + 1]) == "astar") {
				algorithm = 3;
			}
			else {
				exit(-1);
			}
			++i;
		}
		else if (std::string(argv[i]) == "--ss") {
			path = std::string(argv[i + 1]);
			++i;
		}
		else if (std::string(argv[i]) == "--h") {
			heuPath = std::string(argv[i + 1]);
			++i;
		}
		else if (std::string(argv[i]) == "--check-optimistic") {
			checkOptimism = true;
		}
		else if (std::string(argv[i]) == "--check-consistent") {
			checkConsistent = true;
		}
		else {
			exit(-1);
		}
	}

	std::string line;
	std::ifstream SSFile(path);
	std::string start = "";
	std::unordered_set<std::string> stop;
	std::unordered_set<std::string> visited;

	if (algorithm == 1) {
		std::queue<std::tuple<std::string, double, std::string>> que;
		std::unordered_map<std::string, std::set<std::pair<std::string, double>>> stanja;
		while (std::getline(SSFile, line))
		{
			if (line.at(0) == '#') {
				continue;
			}
			if (start == "") {
				start = line;
				que.push(std::tuple<std::string, double, std::string>(start, 0, ""));
				continue;
			}
			if (stop.size() == 0) {
				if (line.find_first_not_of("\t\n ") == std::string::npos) {
					stop.insert(line);
					continue;
				}
				std::istringstream ss(line);
				std::string temp1;
				while (ss >> temp1) {
					stop.insert(temp1);
				}
				continue;
			}
			size_t pos1 = line.find(':');
			std::string temp = line.substr(0, pos1);
			if (pos1 != line.length() - 1) {
				std::string tempString = line.substr(pos1 + 2, line.length() - 1);
				std::istringstream ss(tempString);
				std::string temp1;
				std::set<std::pair<std::string, double>> tempSet;
				while (ss >> temp1)
				{
					size_t pos1 = temp1.find(',');
					tempSet.insert(std::pair<std::string, double>(temp1.substr(0, pos1), stoi(temp1.substr(pos1 + 1, temp1.length() - 1))));
				}
				stanja.insert(std::pair<std::string, std::set<std::pair<std::string, double>>>(temp, tempSet));
			}
			else {
				stanja.insert(std::pair<std::string, std::set<std::pair<std::string, double>>>(temp, std::set<std::pair<std::string, double>>()));
			}
		}

		std::cout << "# BFS " << std::endl;

		std::tuple<std::string, double, std::string> currentNode;
		bool found = false;
		while (!que.empty()) {
			currentNode = que.front();
			que.pop();
			if (visited.find(std::get<0>(currentNode)) != visited.end()) continue;
			visited.insert(std::get<0>(currentNode));
			if (stop.find(std::get<0>(currentNode)) != stop.end()) {
				found = true;
				std::cout << "[FOUND_SOLUTION]: yes" << std::endl;
				std::cout << "[STATES_VISITED]: " << visited.size() << std::endl;
				std::cout << "[PATH_LENGTH]: " << std::count(std::get<2>(currentNode).begin(), std::get<2>(currentNode).end(), '>') + 2<< std::endl;
				std::cout << "[TOTAL_COST]: " << std::get<1>(currentNode) << std::endl;
				std::cout << "[PATH]: " << std::get<2>(currentNode) << " => " << std::get<0>(currentNode) << std::endl;
				break;
			}
			else {
				std::set<std::pair<std::string, double>> goo = stanja.at(std::get<0>(currentNode));
				for (auto a : goo) {
					if (std::get<0>(currentNode) == start) {
						que.push(std::tuple<std::string, double, std::string>(std::get<0>(a), std::get<1>(a) + std::get<1>(currentNode), std::get<0>(currentNode)));
					}
					else {
						que.push(std::tuple<std::string, double, std::string>(std::get<0>(a), std::get<1>(a) + std::get<1>(currentNode), std::get<2>(currentNode) + " => " + std::get<0>(currentNode)));
					}
				}
			}
		}
		if (!found) {
			std::cout << "[FOUND_SOLUTION]: no" << std::endl;
		}
	}


	else if (algorithm == 2) {
		std::priority_queue<std::tuple<double, std::string, std::string>, std::vector<std::tuple<double, std::string, std::string>>, std::greater<std::tuple<double, std::string, std::string>>> que;
		std::unordered_map<std::string, std::set<std::pair<double, std::string>>> stanja;
		while (std::getline(SSFile, line))
		{
			if (line.at(0) == '#') {
				continue;
			}
			if (start == "") {
				start = line;
				que.push(std::tuple<double, std::string, std::string>(0, start, ""));
				continue;
			}
			if (stop.size() == 0) {
				if (line.find_first_not_of("\t\n ") == std::string::npos) {
					stop.insert(line);
					continue;
				}
				std::istringstream ss(line);
				std::string temp1;
				while (ss >> temp1) {
					stop.insert(temp1);
				}
				continue;
			}
			size_t pos1 = line.find(':');
			std::string temp = line.substr(0, pos1);
			if (pos1 != line.length() - 1) {
				std::string tempString = line.substr(pos1 + 2, line.length() - 1);
				std::istringstream ss(tempString);
				std::string temp1;
				std::set<std::pair<double, std::string>> tempSet;
				while (ss >> temp1)
				{
					size_t pos1 = temp1.find(',');
					tempSet.insert(std::pair<double, std::string>(stoi(temp1.substr(pos1 + 1, temp1.length() - 1)), temp1.substr(0, pos1)));
				}
				stanja.insert(std::pair<std::string, std::set<std::pair<double, std::string>>>(temp, tempSet));
			}
			else {
				stanja.insert(std::pair<std::string, std::set<std::pair<double, std::string>>>(temp, std::set<std::pair<double, std::string>>()));
			}
		}

		std::cout << "# UCS " << std::endl;

		std::tuple<double, std::string, std::string> currentNode;
		bool found = false;
		while (!que.empty()) {
			currentNode = que.top();
			que.pop();
			if (visited.find(std::get<1>(currentNode)) != visited.end()) continue;
			visited.insert(std::get<1>(currentNode));
			if (stop.find(std::get<1>(currentNode)) != stop.end()) {
				found = true;
				std::cout << "[FOUND_SOLUTION]: yes" << std::endl;
				std::cout << "[STATES_VISITED]: " << visited.size() << std::endl;
				std::cout << "[PATH_LENGTH]: " << std::count(std::get<2>(currentNode).begin(), std::get<2>(currentNode).end(), '>') + 2 << std::endl;
				std::cout << "[TOTAL_COST]: " << std::get<1>(currentNode) << std::endl;
				std::cout << "[PATH]: " << std::get<2>(currentNode) << " => " << std::get<1>(currentNode) << std::endl;
				break;
			}
			else {
				std::set<std::pair<double, std::string>> goo = stanja.at(std::get<1>(currentNode));
				for (auto a : goo) {
					if (std::get<1>(currentNode) == start) {
						que.push(std::tuple<double, std::string, std::string>(std::get<0>(a) + std::get<0>(currentNode), std::get<1>(a), std::get<1>(currentNode)));
					}
					else {
						que.push(std::tuple<double, std::string, std::string>(std::get<0>(a) + std::get<0>(currentNode), std::get<1>(a), std::get<2>(currentNode) + " => " + std::get<1>(currentNode)));
					}
				}
			}
		}
		if (!found) {
			std::cout << "[FOUND_SOLUTION]: no" << std::endl;
		}
	}

	else if (algorithm == 3) {
		std::priority_queue<std::tuple<double, std::string, std::string>, std::vector<std::tuple<double, std::string, std::string>>, std::greater<std::tuple<double, std::string, std::string>>> que;
		std::unordered_map<std::string, std::set<std::pair<double, std::string>>> stanja;
		while (std::getline(SSFile, line))
		{
			if (line.at(0) == '#') {
				continue;
			}
			if (start == "") {
				start = line;
				que.push(std::tuple<double, std::string, std::string>(0, start, ""));
				continue;
			}
			if (stop.size() == 0) {
				if (line.find_first_not_of("\t\n ") == std::string::npos) {
					stop.insert(line);
					continue;
				}
				std::istringstream ss(line);
				std::string temp1;
				while (ss >> temp1) {
					stop.insert(temp1);
				}
				continue;
			}
			size_t pos1 = line.find(':');
			std::string temp = line.substr(0, pos1);
			if (pos1 != line.length() - 1) {
				std::string tempString = line.substr(pos1 + 2, line.length() - 1);
				std::istringstream ss(tempString);
				std::string temp1;
				std::set<std::pair<double, std::string>> tempSet;
				while (ss >> temp1)
				{
					size_t pos1 = temp1.find(',');
					tempSet.insert(std::pair<double, std::string>(stoi(temp1.substr(pos1 + 1, temp1.length() - 1)), temp1.substr(0, pos1)));
				}
				stanja.insert(std::pair<std::string, std::set<std::pair<double, std::string>>>(temp, tempSet));
			}
			else {
				stanja.insert(std::pair<std::string, std::set<std::pair<double, std::string>>>(temp, std::set<std::pair<double, std::string>>()));
			}
		}

		std::ifstream HFile(heuPath);
		std::unordered_map<std::string, double> heuristics;
		while (std::getline(HFile, line)) {
			size_t pos1 = line.find(':');
			std::string name = line.substr(0, pos1);
			std::string cost1 = line.substr(pos1 + 2, line.length() - 1);
			heuristics.insert({ name, stoi(cost1) });
		}

		std::cout << "# A_STAR " << heuPath << std::endl;

		bool found = false;
		std::tuple<double, std::string, std::string> currentNode;
		while (!que.empty()) {
			currentNode = que.top();
			que.pop();
			if (visited.find(std::get<1>(currentNode)) != visited.end()) continue;
			visited.insert(std::get<1>(currentNode));
			if (stop.find(std::get<1>(currentNode)) != stop.end()) {
				found = true;
				std::cout << "[FOUND_SOLUTION]: yes" << std::endl;
				std::cout << "[STATES_VISITED]: " << visited.size() << std::endl;
				std::cout << "[PATH_LENGTH]: " << std::count(std::get<2>(currentNode).begin(), std::get<2>(currentNode).end(), '>') + 2 << std::endl;
				std::cout << "[TOTAL_COST]: " << std::get<0>(currentNode) << std::endl;
				std::cout << "[PATH]: " << std::get<2>(currentNode) << " => " << std::get<1>(currentNode) << std::endl;
				break;
			}
			else {
				std::set<std::pair<double, std::string>> goo = stanja.at(std::get<1>(currentNode));
				for (auto a : goo) {
					if (std::get<1>(currentNode) == start) {
						que.push(std::tuple<double, std::string, std::string>(std::get<0>(a) + heuristics.at(std::get<1>(a)), std::get<1>(a), std::get<1>(currentNode)));
					}
					else {
						que.push(std::tuple<double, std::string, std::string>(std::get<0>(a) + std::get<0>(currentNode) - heuristics.at(std::get<1>(currentNode)) + heuristics.at(std::get<1>(a)), std::get<1>(a), std::get<2>(currentNode) + " => " + std::get<1>(currentNode)));
					}
				}
			}
		}
		if (!found) {
			std::cout << "[FOUND_SOLUTION]: no" << std::endl;
		}
	}

	if (checkConsistent) {
		std::unordered_map<std::string, std::set<std::pair<std::string, double>>> stanja;
		std::set<std::string> imenaStanja;
		while (std::getline(SSFile, line))
		{
			if (line.at(0) == '#') {
				continue;
			}
			if (start == "") {
				start = line;
				continue;
			}
			if (stop.size() == 0) {
				if (line.find_first_not_of("\t\n ") == std::string::npos) {
					stop.insert(line);
					continue;
				}
				std::istringstream ss(line);
				std::string temp1;
				while (ss >> temp1) {
					stop.insert(temp1);
				}
				continue;
			}
			size_t pos1 = line.find(':');
			std::string temp = line.substr(0, pos1);
			if (pos1 != line.length() - 1) {
				std::string tempString = line.substr(pos1 + 2, line.length() - 1);
				std::istringstream ss(tempString);
				std::string temp1;
				std::set<std::pair<std::string, double>> tempSet;
				while (ss >> temp1)
				{
					size_t pos1 = temp1.find(',');
					tempSet.insert(std::pair<std::string, double>(temp1.substr(0, pos1), stoi(temp1.substr(pos1 + 1, temp1.length() - 1))));
				}
				stanja.insert(std::pair<std::string, std::set<std::pair<std::string, double>>>(temp, tempSet));
				imenaStanja.insert(temp);
			}
			else {
				stanja.insert(std::pair<std::string, std::set<std::pair<std::string, double>>>(temp, std::set<std::pair<std::string, double>>()));
				imenaStanja.insert(temp);
			}
		}

		std::cout << "# HEURISTIC-CONSISTENT " << path << std::endl;

		std::ifstream HFile(heuPath);
		std::unordered_map<std::string, double> heuristics;
		while (std::getline(HFile, line)) {
			size_t pos1 = line.find(':');
			std::string name = line.substr(0, pos1);
			std::string cost1 = line.substr(pos1 + 2, line.length() - 1);
			heuristics.insert({ name, stoi(cost1) });
		}
		double stateHeuristic;
		double childHeuristic;
		bool foundErr = false;
		for (auto state : imenaStanja) {
			stateHeuristic = heuristics.at(state);
			//std::set<std::pair<std::string, int>> djeca = stanja.at(state)
			for (auto childState : stanja.at(state)) {
				childHeuristic = heuristics.at(childState.first);
				if (stateHeuristic <= childHeuristic + childState.second) std::cout << "[CONDITION]: [OK] ";
				else {
					std::cout << "[CONDITION]: [ERR] ";
					foundErr = true;
				}
				std::cout << "h(" << state << ") <= h(" << childState.first << ") + c: " << stateHeuristic << " <= " << childHeuristic << " + " << childState.second << std::endl;
			}
		}

		if (foundErr) {
			std::cout << "[CONCLUSION]: Heuristic is not consistent" << std::endl;
		}
		else {
			std::cout << "[CONCLUSION]: Heuristic is consistent" << std::endl;
		}
	}

	if (checkOptimism) {
		std::priority_queue<std::tuple<double, std::string, std::string>, std::vector<std::tuple<double, std::string, std::string>>, std::greater<std::tuple<double, std::string, std::string>>> que;
		std::unordered_map<std::string, std::set<std::pair<double, std::string>>> stanja;
		std::set<std::string> imenaStanja;
		while (std::getline(SSFile, line))
		{
			if (line.at(0) == '#') {
				continue;
			}
			if (start == "") {
				start = line;
				//que.push(std::tuple<int, std::string, std::string>(0, start, ""));
				continue;
			}
			if (stop.size() == 0) {
				if (line.find_first_not_of("\t\n ") == std::string::npos) {
					stop.insert(line);
					continue;
				}
				std::istringstream ss(line);
				std::string temp1;
				while (ss >> temp1) {
					stop.insert(temp1);
				}
				continue;
			}
			size_t pos1 = line.find(':');
			std::string temp = line.substr(0, pos1);
			if (pos1 != line.length() - 1) {
				std::string tempString = line.substr(pos1 + 2, line.length() - 1);
				std::istringstream ss(tempString);
				std::string temp1;
				std::set<std::pair<double, std::string>> tempSet;
				while (ss >> temp1)
				{
					size_t pos1 = temp1.find(',');
					tempSet.insert(std::pair<double, std::string>(stoi(temp1.substr(pos1 + 1, temp1.length() - 1)), temp1.substr(0, pos1)));
				}
				stanja.insert(std::pair<std::string, std::set<std::pair<double, std::string>>>(temp, tempSet));
				imenaStanja.insert(temp);
			}
			else {
				stanja.insert(std::pair<std::string, std::set<std::pair<double, std::string>>>(temp, std::set<std::pair<double, std::string>>()));
				imenaStanja.insert(temp);
			}
		}

		std::cout << "# HEURISTIC-CONSISTENT " << path << std::endl;

		std::ifstream HFile(heuPath);
		std::unordered_map<std::string, double> heuristics;
		while (std::getline(HFile, line)) {
			size_t pos1 = line.find(':');
			std::string name = line.substr(0, pos1);
			std::string cost1 = line.substr(pos1 + 2, line.length() - 1);
			heuristics.insert({ name, stoi(cost1) });
		}
		bool foundErr = false;
		std::tuple<double, std::string, std::string> currentNode;
		for (auto startState : imenaStanja) {
			que.push(std::tuple<double, std::string, std::string>(0, startState, ""));
			double stateHeu = heuristics.at(startState);
			while (!que.empty()) {
				currentNode = que.top();
				que.pop();
				if (visited.find(std::get<1>(currentNode)) != visited.end()) continue;
				visited.insert(std::get<1>(currentNode));
				if (stop.find(std::get<1>(currentNode)) != stop.end()) {
					//std::cout << "Cijena: " << std::get<0>(currentNode) << std::endl;
					//std::cout << "Put: " << std::get<2>(currentNode) << " => " << std::get<1>(currentNode) << std::endl;
					if (stateHeu <= std::get<0>(currentNode)) {
						std::cout << "[CONDITION]: [OK] ";
					}
					else {
						foundErr = true;
						std::cout << "[CONDITION]: [ERR] ";
					}
					std::cout << "h(" << startState << ") <= h*: " << stateHeu << " <= " << std::get<0>(currentNode) << std::endl;
					break;
				}
				else {
					std::set<std::pair<double, std::string>> goo = stanja.at(std::get<1>(currentNode));
					for (auto a : goo) {
						if (std::get<1>(currentNode) == start) {
							que.push(std::tuple<double, std::string, std::string>(std::get<0>(a) + std::get<0>(currentNode), std::get<1>(a), std::get<1>(currentNode)));
						}
						else {
							que.push(std::tuple<double, std::string, std::string>(std::get<0>(a) + std::get<0>(currentNode), std::get<1>(a), std::get<2>(currentNode) + " => " + std::get<1>(currentNode)));
						}
					}
				}
			}
			visited.clear();
			while (!que.empty()) {
				que.pop();
			}
		}
		std::cout << (foundErr ? "[CONCLUSION]: Heuristic is not optimistic" : "[CONCLUSION]: Heuristic is optimistic") << std::endl;
	}
}