#include "Character.h"
#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

Character* findCharacter(vector<Character> &community, string name);

void sort(vector<Character>& v);

string findNext(vector<Character> community, string name);

int main(int argc, char* argv[]) {
     
    string infile_name = argv[1];
	string outfile_name = argv[2];

	ifstream infile;
	infile.open(infile_name);

	ofstream outfile;
	outfile.open(outfile_name); 

	vector<Character>  community1;
	vector<Character> community2;

	int nMaxRounds;

	infile >> nMaxRounds;

	for (int i = 0 ; i < 5 ; i++)
	{
        string name, type;
	    int attack, defense, health;
	    infile >> name >> type >> attack >> defense >> health;
	    community1.push_back(Character(name, type, attack, defense, health, nMaxRounds));
	}

	for (int i = 0 ; i < 5 ; i++)
	{
        string name, type;
	    int attack, defense, health;
	    infile >> name >> type >> attack >> defense >> health;
	    community2.push_back(Character(name, type, attack, defense, health, nMaxRounds));
	}

	int nRoundsPassed = 0;
	int numDeaths = 0;
	string result;

	string lastKilled1; //holds the name of the lastly killed character of community1
	string lastKilled2; //holds the name of the lastly killed character of community2


	for (int i = 0; i < nMaxRounds; i++)
	{
		string attacker_name, defender_name, if_special;
		infile >> attacker_name >> defender_name >> if_special;

		Character* attacker; 
		Character* defender;
		string team; //The team who attacks, therefore has chance to win at that round

		if(nRoundsPassed % 2 == 0){
			attacker = findCharacter(community1, attacker_name);
			defender = findCharacter(community2, defender_name);
			team = "Community-1";
		} else{
			attacker = findCharacter(community2, attacker_name);
			defender = findCharacter(community1, defender_name);
			team = "Community-2";
		}

		if (if_special == "SPECIAL" && ((attacker->type == "Elves" && attacker->nRoundsSinceSpecial >= 11) || 
		(attacker->type == "Dwarfs" && attacker->nRoundsSinceSpecial >= 21) || (attacker->type == "Wizards" && attacker->nRoundsSinceSpecial >= 51))){ 

		if(attacker->type == "Elves"){ 

			for (int i = 0; i < 5; i++)
			{
				if(community1[i].type == "Hobbit"){ 
					int transport = attacker->remainingHealth / 2;
					community1[i].remainingHealth += transport;
					attacker->remainingHealth -= transport;
	
					break;
				}
			}
			attacker->nRoundsSinceSpecial = 0;
		}
		else if(attacker->type == "Dwarfs"){
			defender->remainingHealth -= (attacker->attack - defender->defense);
			attacker->nRoundsSinceSpecial = 0;
		}
		else if(attacker->type == "Wizards"){    
			if(team == "Community-1"){
				if(lastKilled1 != ""){
					int index;
					for (int i = 0; i < community1.size(); i++)
					{
						if(community1[i].name == lastKilled1){
							index = i;
							break;
						}
					}
					if(community1[index].isAlive){
						community1[index].remainingHealth = community1[index].healthHistory[0];
					} else {
		    			community1[index].isAlive = true;
						community1[index].nRoundsSinceSpecial = 0;
						community1[index].remainingHealth = community1[index].healthHistory[0];
						numDeaths--;
					}
					attacker->nRoundsSinceSpecial = 0;
				}
			} 
			else if(team == "Community-2"){
			if(lastKilled2 != ""){ 
					int index;
					for (int i = 0; i < community2.size(); i++)
					{
						if(community2[i].name == lastKilled2){
							index = i;
							break;
						}
					}
					if(community2[index].isAlive){
						community2[index].remainingHealth = community2[index].healthHistory[0];
					} else {
		    			community2[index].isAlive = true;
						community2[index].nRoundsSinceSpecial = 0;
						community2[index].remainingHealth = community2[index].healthHistory[0];
						numDeaths--;
					}
					attacker->nRoundsSinceSpecial = 0;
				}
		}
		}

	}
int damage = attacker->attack - defender->defense;
	if(damage > 0){
	defender->remainingHealth -= damage;
}

	if(defender->remainingHealth <= 0){
		defender->remainingHealth = 0;
		defender->isAlive = false;
		if(team == "Community-1"){
			lastKilled2 = defender->name;
		}
		else {
			lastKilled1 = defender->name;
		}
		numDeaths++;
	}

	nRoundsPassed++;

	for (int i = 0; i < 5; i++)
	{
		community1[i].healthHistory[nRoundsPassed] = community1[i].remainingHealth;
		if(community1[i].isAlive){
		if(community1[i].type == "Elves" || community1[i].type == "Dwarfs" || community1[i].type == "Wizards"){ 
			community1[i].nRoundsSinceSpecial += 1;
		}
	}
	}
     
	for (int i = 0; i < 5; i++) 
	{
		community2[i].healthHistory[nRoundsPassed] = community2[i].remainingHealth;
		if(community2[i].isAlive){
		if(community2[i].type == "Elves" || community2[i].type == "Dwarfs" || community2[i].type == "Wizards"){
			community2[i].nRoundsSinceSpecial += 1;
		}
	}
	}

	if(!defender->isAlive){
		if(defender->type == "Hobbit"){
			result = team;
		} else {   //This else part checks whether the other characters except Hobbit died or not
			bool isAllDead = true;
			if(team == "Community-1"){   
				for(int i = 0; i < 5; i++){
		        if(community2[i].type != "Hobbit" && community2[i].isAlive){ 
			    isAllDead = false;
			    break;
		}
	}
	if(isAllDead){
		result = team;
	}
			}
			else if(team == "Community-2"){
				for(int i = 0; i < 5; i++){
		        if(community1[i].type != "Hobbit" && community1[i].isAlive){
			    isAllDead = false;
			    break;
		}
	}
	  if(isAllDead){
		result = team;
	 } 
    }
		}
	}
		if (result != "")
		{
			break;
		}
	}

	if(result == ""){
		result = "Draw";
	}

	outfile << result << "\n" << nRoundsPassed << "\n" << numDeaths << endl;

	for (int i = 0; i < 5; ++i)
	{
		outfile << community1[i].name + " ";
		int size = nRoundsPassed + 1; 

		for (int j = 0; j < size ; j++)
		 {
		 	outfile << community1[i].healthHistory[j] << " ";
		 }

		 outfile << endl;
		
	}
	for (int i = 0; i < 5; ++i)
	{
		outfile << community2[i].name  << " ";
		int size = nRoundsPassed + 1; 

		for (int j = 0; j < size ; j++)
		 {
		 	outfile << community2[i].healthHistory[j] << " ";
		 }
		 outfile << endl;
	}

	infile.close();
	outfile.close();
    return 0;
}

Character* findCharacter(vector<Character> &community, string name){
	for (int i = 0; i < 5; i++)
	{
		if(community[i].name == name){
			if(community[i].isAlive){
				return &community[i];
			}
		}
	}

	string nextAlive = findNext(community, name); //Holds the next alive character's name

	for (int i = 0; i < 5; i++)
	{
		if(community[i].name == nextAlive){
				return &community[i];
		}
}
	return NULL;
}

void sort(vector<Character>& v){ //Sorts the vector of characters according to their names

	for (int i = 0; i < v.size(); i++)
	{
		for(int j = i; j < v.size(); j++){
			if(v[j] < v[i]){
				Character temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}
		}
	}
}

string findNext(vector<Character> community, string name){ //Returns the name of the next alive character which is alphabetically comes next after name
	sort(community);
	int index;
	for (int i = 0; i < community.size(); i++)
	{
		if(community[i].name == name){
				index = i;
				break;
		}
}
	for (int i = index ; i < community.size() ; i++)
	{
		if (community[i].isAlive)
		{
			return community[i].name;
		}
	}

	for (int i = index; i >= 0 ; i--)
	{
		if(community[i].isAlive)
		{
			return community[i].name;
		}
	}
	return "";
}