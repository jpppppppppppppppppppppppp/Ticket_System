#include "BPT.hpp"
#include "map.hpp"
#include <iostream>
using std::cout;
using std::cin;

std::string timestamp;
std::string opt,opt2;
struct user{
	my_string<35>password;
	my_string<15>name;
	my_string<35>mailAddr;
	int privilege=0;
	user(){};
	user(std::string & pp, std::string & nn, std::string & mm, int gg){
		strcpy(password.ch, pp.c_str());
		strcpy(name.ch, nn.c_str());
		strcpy(mailAddr.ch, mm.c_str());
		privilege = gg;
	}
	bool friend operator < (const user & lhs, const user & rhs){
		return true;
	}
	bool friend operator == (const user & lhs, const user & rhs){
		return true;
	}
};
BPT<my_string<25>,user>userbank("userbank");
sjtu::map<my_string<25>, int>loginuser;




int main(){
	while (true){
		if(std::cin.eof()){
			break;
		}else{
			cin >> timestamp;
			cin >> opt;
			if(opt == "add_user"){
				std::string cc,uu,pp,nn,mm;
				bool flag = true;
				int gg=0,cgg=0;
				for(int i = 0; i < 6; ++i){
					cin >> opt2;
					if(opt2 == "-c"){
						cin >> cc;
						my_string<25>cur_user(cc);
						auto ans = userbank.Find(cur_user);
						if(!userbank.empty()){
							if(ans.empty())flag = false;
							else cgg = ans.front()->privilege;
						}
					}
					else if(opt2 == "-u"){
						cin >> uu;
					}
					else if(opt2 == "-p"){
						cin >> pp;
					}
					else if(opt2 == "-n"){
						cin >> nn;
					}
					else if(opt2 == "-m"){
						cin >> mm;
					}
					else if(opt2 == "-g"){
						cin >> gg;
						if(gg < 0 or gg > 10)flag = false;
					}
					else flag = false;
				}
				if(!userbank.empty() and cgg <= gg)flag = false;
				if(userbank.empty())gg = 10;
				my_string<25>user_name(uu);
				if(!userbank.Find(user_name).empty())flag = false;
				if(flag){
					user new_user(pp,nn,mm,gg);
					userbank.Insert(user_name, new_user);
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "login"){
				std::string uu,pp;
				bool flag = true;
				for(int i = 0; i < 2; ++i){
					cin >> opt2;
					if(opt2 == "-u"){
						cin >> uu;
					}
					else if(opt2 == "-p"){
						cin >> pp;
					}
					else flag = false;
				}
				my_string<25>user_name(uu);
				auto ans = userbank.Find(user_name);
				if(!ans.empty() and ans.front()->password == pp and loginuser.find(user_name)==loginuser.end() and flag){
					loginuser[user_name] = ans.front()->privilege;
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "logout"){
				std::string uu;
				cin >> opt2 >> uu;
				my_string<25>user_name(uu);
				auto iter = loginuser.find(user_name);
				if(iter!=loginuser.end()){
					loginuser.erase(iter);
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "query_profile"){
				std::string cc,uu;
				for(int i = 0; i < 2; ++i){
					cin >> opt2;
					if(opt2 == "-c"){
						cin >> cc;
					}else if(opt2 == "-u"){
						cin >> uu;
					}
				}
				bool flag = true;
				my_string<25>user_name(uu);
				my_string<25>cur_name(cc);
				if(loginuser.find(cur_name)==loginuser.end())flag= false;
				auto ans = userbank.Find(user_name);
				if(ans.empty())flag = false;
				if(flag){
					if(ans.front()->privilege <= loginuser[cur_name]){
						user* u = ans.front();
						cout << timestamp << ' ' << uu << ' ' << u->name << ' ' << u->mailAddr << ' ' << u->privilege << '\n';
					}
					else cout << timestamp << " -1\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "modify_profile"){
				std::string line;
				getline(std::cin,line);
				line.append(" ");
				std::string cc,uu,pp,nn,mm;int gg=0;
				opt2.clear();
				for(int i = 1; i < line.length(); ++i){
					if(line[i] == ' '){
						if(opt2 == "-c"){
							while (line[++i] != ' '){
								cc += line[i];
							}
						}
						else if(opt2 == "-u"){
							while (line[++i] != ' '){
								uu += line[i];
							}
						}
						else if(opt2 == "-p"){
							while (line[++i] != ' '){
								pp += line[i];
							}
						}
						else if(opt2 == "-n"){
							while (line[++i] != ' '){
								nn += line[i];
							}
						}
						else if(opt2 == "-m"){
							while (line[++i] != ' '){
								mm += line[i];
							}
						}
						else if(opt2 == "-g"){
							gg = line[++i] - '0';
						}
						opt2.clear();
					}
					else{
						opt2 += line[i];
					}
				}

			}
			else if(opt == "exit"){
				cout << timestamp << " bye\n";
				break;
			}else return -1;
		}
	}
	return 0;
} 