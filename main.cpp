#include "BPT.hpp"
#include "map.hpp"
#include <iostream>
#include <cstring>
using std::cout;
using std::cin;

inline int str_to_int(std::string & str){
	if(str == "_")return 0;
	int ans = 0;
	for(int i = 0; i < str.length(); ++i){
		ans = ans * 10 + str[i] - '0';
	}
	return ans;
}
sjtu::vector<std::string> parser(const std::string & str){
	sjtu::vector<std::string>ans;
	std::string buffer;
	for(auto ch:str){
		if(ch == '|'){
			ans.push_back(buffer);
			buffer.clear();
		}else buffer += ch;
	}
	return ans;
}



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
	inline bool friend operator < (const user & lhs, const user & rhs){
		return true;
	}
	inline bool friend operator == (const user & lhs, const user & rhs){
		return true;
	}
};
BPT<my_string<25>,user>userbank("userbank");
sjtu::map<my_string<25>, int>loginuser;
struct my_time{
	int hh=0,mi=0;
	my_time(){}
	my_time(std::string & str){
		std::string str1,str2;
		int i=0;
		while (str[i]!=':')str1 += str[i++];
		++i;
		while (i<str.length())str2 += str[i++];
		hh = str_to_int(str1);
		mi = str_to_int(str2);
	}
	my_time(int x,int y){hh=x,mi=y;}
	inline my_time add_mi(int x) const{
		return my_time(hh + (mi+x)/60, (mi+x)/60);
	}
	inline void add_hh(int x){
		hh += x;
	}
	inline void add_hhmi(int x, int y){
		mi += y;
		hh += x + (mi / 60);
		mi /= 60;
	}
	inline void operator = (my_time t){
		hh = t.hh;
		mi = t.mi;
	}
};
struct my_date{
	int mm=0,dd=0;
	my_date(){}
	my_date(std::string & str){
		std::string str1,str2;
		int i=0;
		while (str[i]!='-')str1 += str[i++];
		++i;
		while (i<str.length())str2 += str[i++];
		mm = str_to_int(str1);
		dd = str_to_int(str2);
	}
	my_date(int x,int y){
		mm = x;
		dd = y;
	}
	my_date& add(int x){
		dd += x;
		if(mm == 6 and dd >= 31)mm = 7, dd -= 30;
		if(mm == 7 and dd >= 32)mm = 8, dd -= 31;
		return *this;
	}
	my_date add_time(my_time & t){
		int d = dd + t.hh / 24;
		int m = mm;
		if(mm == 6 and dd >= 31)mm = 7, dd -= 30;
		if(mm == 7 and dd >= 32)mm = 8, dd -= 31;
		return my_date(m,d);
	}
	inline friend bool operator < (const my_date & lhs,const my_date & rhs){
		if(lhs.mm != rhs.mm)return lhs.mm < rhs.mm;
		return lhs.dd < rhs.dd;
	}
	inline friend bool operator == (const my_date & lhs,const my_date & rhs){
		return lhs.mm == rhs.mm and lhs.dd == rhs.dd;
	}
};
inline int between(const my_date& lhs, const my_date& rhs){
	int ans = 0;
	if(rhs.mm == 6)ans += 0;
	else if(rhs.mm == 7)ans += 30;
	else if(rhs.mm == 8)ans += 61;
	if(lhs.mm == 6)ans -= 0;
	else if(lhs.mm == 7)ans -= 30;
	else if(lhs.mm == 8)ans -= 61;
	ans += rhs.dd - lhs.dd;
	return ans;
}
struct date_time{
	int mm,dd,hh,mi;
};
struct train {
	int stationNum = 0;
	my_string<25> stations[110];
	int seatNum = 0;
	int prices[110]{};
	char type = 0;
	my_time arriveTime[110];
	my_time leaveTime[110];
	my_date beginSale;
	my_date endSale;
	bool is_release = false;
	train(){
		memset(arriveTime,0,sizeof(arriveTime));
		memset(leaveTime,0,sizeof(leaveTime));
		memset(prices,0,sizeof(prices));
		memset(stations, 0,sizeof(stations));
	}
};
BPT<my_string<25>,train>trainbank("trainbank");
struct trainid{
	my_string<25>trainID;
	int num=0;
	inline friend bool operator < (const trainid & lhs,const trainid & rhs){return lhs.trainID < rhs.trainID;}
	inline friend bool operator == (const trainid & lhs,const trainid & rhs){return false;}
};
Multiple_BPT<my_string<25>,trainid>stations("allstation");
struct dateAndtrainID{
	my_date startDate;
	my_string<25>trainID;
	dateAndtrainID(){}
	dateAndtrainID(my_date&date,my_string<25>&str){
		startDate = date;
		trainID = str;
	}
	inline friend bool operator < (const dateAndtrainID & lhs,const dateAndtrainID & rhs){
		if(!(lhs.startDate==rhs.startDate))return lhs.startDate < rhs.startDate;
		return lhs.trainID < rhs.trainID;
	}
	inline friend bool operator == (const dateAndtrainID & lhs,const dateAndtrainID & rhs){
		return lhs.startDate == rhs.startDate and lhs.trainID == rhs.trainID;
	}
};
struct seats{
	int seat[110]{};
	seats(){ memset(seat,0,sizeof(seat));}
	seats(int stationNum, int seatNum){
		memset(seat,0,sizeof(seat));
		for(int i = 0; i < stationNum; ++i){
			seat[i] = seatNum;
		}
	}
};
BPT<dateAndtrainID,seats>allseats("seats");

int main(){
	freopen("../testcases/basic_2/1.in","r",stdin);
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
							if(ans == nullptr)flag = false;
							else cgg = ans->privilege;
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
				if(userbank.Find(user_name)!= nullptr)flag = false;
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
				if(ans!= nullptr and ans->password == pp and loginuser.find(user_name)==loginuser.end() and flag){
					loginuser[user_name] = ans->privilege;
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
				if(ans== nullptr)flag = false;
				if(flag){
					if(ans->privilege < loginuser[cur_name] or cc == uu){
						user* u = ans;
						cout << timestamp << ' ' << uu << ' ' << u->name << ' ' << u->mailAddr << ' ' << u->privilege << '\n';
					}
					else cout << timestamp << " -1\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "modify_profile"){
				std::string line;
				getline(std::cin,line);
				line.append(" ");
				std::string cc,uu,pp,nn,mm;int gg=-1;
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
				my_string<25>cur_name(cc),user_name(uu);
				auto ans = userbank.Find(user_name);
				if(loginuser.find(cur_name)!=loginuser.end() and ans!= nullptr and (loginuser[cur_name]>ans->privilege or cc == uu) and gg<loginuser[cur_name]){
					user * u = ans;
					if(!pp.empty())strcpy(u->password.ch,pp.c_str());
					if(!nn.empty())strcpy(u->name.ch,nn.c_str());
					if(!mm.empty())strcpy(u->mailAddr.ch,mm.c_str());
					if(gg>=0)u->privilege = gg;
					if(loginuser.find(user_name)!=loginuser.end())loginuser[user_name] = u->privilege;
					cout << timestamp << ' ' << uu << ' ' << u->name << ' ' << u->mailAddr << ' ' << u->privilege << '\n';
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "add_train"){
				sjtu::map<std::string,std::string>information;
				std::string infor;
				for(int i = 0; i < 10; ++i){
					cin >> opt2;
					cin >> infor;
					information[opt2] = infor;
				}
				my_string<25>trainID(information["-i"]);
				if(trainbank.Find(trainID)== nullptr){
					train new_train;
					new_train.stationNum = str_to_int(information["-n"]);
					new_train.seatNum = str_to_int(information["-m"]);
					new_train.leaveTime[0] = my_time(information["-x"]);
					new_train.arriveTime[0] = my_time(-1,-1);
					new_train.type = information["-y"][0];
					sjtu::vector<std::string> ss = parser(information["-s"] + '|');
					sjtu::vector<std::string> pp = parser(information["-p"] + '|');
					sjtu::vector<std::string> tt = parser(information["-t"] + '|');
					sjtu::vector<std::string> oo = parser(information["-o"] + '|');
					sjtu::vector<std::string> dd = parser(information["-d"] + '|');
					new_train.beginSale = my_date(dd[0]);
					new_train.endSale = my_date(dd[1]);
					new_train.stations[0] = ss[0];
					int i=1;
					for(; i < new_train.stationNum-1; ++i){
						new_train.stations[i] = ss[i];
						new_train.arriveTime[i] = new_train.leaveTime[i-1].add_mi(str_to_int(tt[i-1]));
						new_train.leaveTime[i] = new_train.arriveTime[i].add_mi(str_to_int(oo[i-1]));
						new_train.prices[i] = new_train.prices[i-1] + str_to_int(pp[i-1]);
					}
					new_train.prices[i] = new_train.prices[i-1] + str_to_int(pp[i-1]);
					new_train.stations[i] = ss[i];
					new_train.arriveTime[i] = new_train.leaveTime[i-1].add_mi(str_to_int(tt[i-1]));
					new_train.leaveTime[i] = my_time(-1,-1);
					trainbank.Insert(trainID,new_train);
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "release_train"){
				cin >> opt2;
				std::string infor;
				cin >> infor;
				my_string<25>trainID(infor);
				train* tr = trainbank.Find(trainID);
				if(tr != nullptr and !tr->is_release){
					tr->is_release = true;
					for(int i = 0; i < tr->stationNum; ++i){
						stations.Insert(tr->stations[i],{trainID,i});
					}
					my_date useless = tr->beginSale;
					for(int i = 0; i <= between(tr->beginSale,tr->endSale); ++i){
						dateAndtrainID dat(useless, trainID);
						allseats.Insert(dat, seats(tr->stationNum, tr->seatNum));
						useless.add(1);
					}
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "buy_ticket"){
				std::string line;
				getline(std::cin, line);
				line.append(" ");
				std::string uu,ii,dd,nn,ff,tt,qq;
				opt2.clear();
				for(int i = 0; i < line.length(); ++i){
					if(line[i] == ' '){
						if(opt2 == "-u"){
							while (line[++i]!=' '){
								uu += line[i];
							}
						}else if(opt2 == "-i"){
							while (line[++i]!=' '){
								ii += line[i];
							}
						}else if(opt2 == "-d"){
							while (line[++i]!=' '){
								dd += line[i];
							}
						}else if(opt2 == "-n"){
							while (line[++i]!=' '){
								nn += line[i];
							}
						}else if(opt2 == "-f"){
							while (line[++i]!=' '){
								ff += line[i];
							}
						}else if(opt2 == "-t"){
							while (line[++i]!=' '){
								tt += line[i];
							}
						}else if(opt2 == "-q"){
							while (line[++i]!=' '){
								qq += line[i];
							}
						}
						opt2.clear();
					}else opt2 += line[i];
				}
				bool q = false;
				if(!qq.empty() and qq[0] == 't')q = true;
				my_string<25>trainID(ii);
				my_string<25>username(uu);
				bool flag = true;
				train * tr = trainbank.Find(trainID);
				my_string<25>from(ff);
				my_string<25>to(tt);
				int ind1=0,ind2=0;
				if(loginuser.find(username)!=loginuser.end()){
					if(tr == nullptr)flag = false;
					else{
						if(!tr->is_release)flag = false;
						else{
							ind1 = lower_bound(tr->stations,tr->stations + tr->stationNum, from) - tr->stations;
							ind2 = lower_bound(tr->stations,tr->stations + tr->stationNum, to) - tr->stations;
							if(!(tr->stations[ind1] == from))flag = false;
							if(!(tr->stations[ind2] == to))flag = false;
							if(flag and ind1 >= ind2)flag = false;
						}
					}
				}else flag = false;
				bool time_val = false;
				if(flag){
					my_date useless = tr->beginSale;
					my_date dateNeed(dd);
					for(int i = 0; i <= between(tr->beginSale,tr->endSale); ++i){
						if(useless.add_time(tr->leaveTime[ind1]) == dateNeed){
							time_val = true;
							break;
						}
						if(dateNeed < useless.add_time(tr->leaveTime[ind1])){
							flag = false;
							break;
						}
						useless.add(1);
					}
					if(time_val){
						int seatNeed = str_to_int(nn);
						dateAndtrainID dat(useless, trainID);
						seats* s = allseats.Find(dat);
						int min_seat = tr->seatNum;
						for(int i = ind1; i < ind2; ++i){
							if(s->seat[i] < min_seat)min_seat = s->seat[i];
						}
						if(min_seat >= seatNeed){
							for(int i = ind1; i < ind2; ++i){
								s->seat[i] -= seatNeed;
							}

						}
					}
				}

				cout << timestamp << " -1\n";
			}
			else if(opt == "exit"){
				cout << timestamp << " bye\n";
				break;
			}else return -1;
		}
	}
	return 0;
} 