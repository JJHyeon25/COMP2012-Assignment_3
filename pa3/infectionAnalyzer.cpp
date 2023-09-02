//this is one of the two files that you need to submit

#include "infectionAnalyzer.h"
#include <fstream> //for reading file
#include <queue> //you may use STL queue
#include <algorithm> //you may use STL algorithm
using namespace std;
//you are NOT allowed to include additional libraries yourself

InfectionAnalyzer::~InfectionAnalyzer(){
	vector<Tree<string>>::iterator p;
	for (unsigned int p = 0; p < trees.size(); ++p){
		delete trees[p];
	}

	trees.clear();
}

bool InfectionAnalyzer::loadInfectionFile(const string filename){
	ifstream inf(filename);
	if (!inf)
		return false;

	string acc, vic;

	while(inf >> acc >> vic){
		if (!isInfected(acc) && !isInfected(vic)){
			Tree<string>* input = new Tree<string>;
			input->addRoot(acc);
			input->addChild(acc,vic);
			trees.push_back(input);
		}
		else if (isInfected(acc) && !isInfected(vic)){
			for (unsigned int p = 0; p < trees.size(); ++p){
				if(trees[p]->find(acc)){
					trees[p]->addChild(acc,vic);
					break;
				}
			}
		}
		else if (!isInfected(acc) && isInfected(vic)){
			for (unsigned int p = 0; p < trees.size(); ++p){
				if(trees[p]->find(vic)){
					trees[p]->addRoot(acc);
					break;
				}
			}
		}
		else{
			for (unsigned int p = 0; p < trees.size(); ++p){
				if(trees[p]->find(acc)){
					for (unsigned int q = 0; q < trees.size(); ++q){
						if(trees[q]->find(vic)){
							trees[p]->find(acc)->addChild(acc,vic);
							trees[p]->find(vic)->root = trees[q]->find(vic)->root;
							trees.erase(trees.begin() + q);
							break;
						}
					}
				}
			}
		}
	}

	inf.close();
	return true;
}

bool InfectionAnalyzer::isInfected(const string name) const{
	for (unsigned int p = 0; p < trees.size(); ++p){
		if(trees[p]->find(name) != nullptr)
			return true;
	}

	return false;
}

string InfectionAnalyzer::getInfectionSource(const string name) const{
	if(!isInfected(name))
		return "NA";

	if(!getInfectionGeneration(name)){
		return "ZERO";
	}

	vector<Tree<string>*> list;
	string father;

	for (unsigned int q = 0; q < trees.size(); ++q){
		list.push_back(trees[q]);
	}
	for(unsigned int i = 0; i < list.size(); ++i){
		for(int j = 0; j < list[i]->root->childCount; ++j){
			list.push_back(&list[i]->root->children[j]);
		}
	}

	for(unsigned int i = 0; i < list.size(); ++i){
		for(int j = 0; j < list[i]->root->childCount; ++j){
			if (list[i]->root->children[j].root->data == name){
				father = list[i]->root->data;
			}
		}
	}

	list.clear();

	return father;
}

int InfectionAnalyzer::getInfectionGeneration(const string name) const{
	if(!isInfected(name))
		return -1;

	for (unsigned int p = 0; p < trees.size(); ++p){
		if(trees[p]->find(name) != nullptr){
			return trees[p]->getDepth(name);
		}
	}

	return -1;
}

bool intcompare(pair<string, int> i, pair<string, int> j){
	return(i.second > j.second);
}

bool strcompare(pair<string, int> i, pair<string, int> j){
	return (i.first < j.first);
}

unsigned int findindex(vector<pair<string, int>>& list, unsigned int i){
	unsigned int j{}, k{i};

	for (j = i + 1; j < list.size(); ++j){
		if(list[i].second == list[j].second)
			k = j;
	}

	return k;
}

const vector<pair<string, int>>& InfectionAnalyzer::getInfectionPowerVector(){
	infectionPowerVector.clear();

	vector<Tree<string>*> list;

	for (unsigned int q = 0; q < trees.size(); ++q){
		list.push_back(trees[q]);
	}
	for(unsigned int i = 0; i < list.size(); ++i){
		for(int j = 0; j < list[i]->root->childCount; ++j){
			list.push_back(&list[i]->root->children[j]);
		}
	}

	for (unsigned int p = 0; p < list.size(); ++p){
		infectionPowerVector.push_back(make_pair(list[p]->root->data, list[p]->getDescendantCount()));
	}

	sort(infectionPowerVector.begin(),infectionPowerVector.end(),intcompare);
	for(unsigned int i = 0; i < infectionPowerVector.size(); ++i){
		unsigned int j = findindex(infectionPowerVector, i);
		if (j == i)
			continue;
		sort(infectionPowerVector.begin() + i, infectionPowerVector.begin() + j + 1,strcompare);
		i = j;
	}

	list.clear();

	return infectionPowerVector;
}
