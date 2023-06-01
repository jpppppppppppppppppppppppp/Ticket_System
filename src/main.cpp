#include "BPT.hpp"
#include "map.hpp"
#include <iostream>
#include <cstring>
using std::cout;
using std::cin;
std::hash<std::string> hash;
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
	my_string<20>name;
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
BPT_old<size_t,user>userbank("userbank");
sjtu::map<size_t, int>loginuser;
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
		return my_time(hh + (mi+x)/60, (mi+x)%60);
	}
	inline void add_hh(int x){
		hh += x;
	}
	inline void add_hhmi(int x, int y){
		mi += y;
		hh += x + (mi / 60);
		mi %= 60;
	}
	inline my_time & operator = (my_time t){
		hh = t.hh;
		mi = t.mi;
		return *this;
	}
	int to_ind(){
		return hh * 60 + mi;
	}
};
inline bool compare(const my_time & lhs, const my_time & rhs){
	if(lhs.hh % 24 != rhs.hh % 24)return lhs.hh % 24 > rhs.hh % 24;
	return lhs.mi % 60 > rhs.mi % 60;
}
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
	inline my_date& add(int x){
		dd += x;
		if(mm == 6 and dd >= 31)mm = 7, dd -= 30;
		if(mm == 7 and dd >= 32)mm = 8, dd -= 31;
		return *this;
	}
	inline my_date add_time(my_time & t){
		int d = dd + t.hh / 24;
		int m = mm;
		if(m == 6 and d >= 31)m = 7, d -= 30;
		if(m == 7 and d >= 32)m = 8, d -= 31;
		return my_date(m,d);
	}
	inline my_date minus_time(my_time & t){
		int d = dd - t.hh / 24;
		int m = mm;
		if(m == 8 and d <= 0)m = 7, d += 31;
		if(m == 7 and d <= 0)m = 6, d += 30;
		return my_date(m,d);
	}
	inline friend bool operator < (const my_date & lhs,const my_date & rhs){
		if(lhs.mm != rhs.mm)return lhs.mm < rhs.mm;
		return lhs.dd < rhs.dd;
	}
	inline friend bool operator == (const my_date & lhs,const my_date & rhs){
		return lhs.mm == rhs.mm and lhs.dd == rhs.dd;
	}
	inline friend bool operator <= (const my_date & lhs,const my_date & rhs){
		return lhs == rhs or lhs < rhs;
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
inline int mid(const my_time& lhs, const my_time& rhs){
	return (rhs.hh - lhs.hh) * 60 + rhs.mi - lhs.mi;
}
struct date_time{
	int mm=0,dd=0,hh=0,mi=0;
	date_time(){}
	date_time(my_date& mydate, my_time& mytime){
		mm = mydate.mm;
		dd = mydate.dd;
		hh = mytime.hh;
		mi = mytime.mi;
		if(hh >= 0 and mi >=0){
			dd += hh / 24;
			hh %= 24;
			if(mm == 6 and dd >= 31)mm = 7, dd -= 30;
			if(mm == 7 and dd >= 32)mm = 8, dd -= 31;
			if(mm == 8 and dd >= 32)mm = 9, dd -= 31;
		}
	}
	inline friend std::ostream& operator << (std::ostream & io, const date_time& me){
		if(me.hh >=0 and me.mi >=0){
			io << '0' << me.mm << '-';
			if(me.dd < 10)io << '0';
			io << me.dd << ' ';
			if(me.hh < 10)io << '0';
			io << me.hh << ':';
			if(me.mi < 10)io << '0';
			io << me.mi;
			return io;
		}
		else{
			io << "xx-xx xx:xx";
			return io;
		}
	}
};
struct train {
	int stationNum = 0;
	my_string<35> stations[110];
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
BPT<size_t,size_t,train>trainbank("trainbank");
struct trainid{
	my_string<25>trainID;
	int num=0;
	trainid(){}
	trainid(my_string<25>&tr, int n){
		trainID = tr;
		num = n;
	}
	inline friend bool operator < (const trainid & lhs,const trainid & rhs){return lhs.trainID < rhs.trainID;}
	inline friend bool operator == (const trainid & lhs,const trainid & rhs){return lhs.trainID == rhs.trainID and lhs.num == rhs.num;}
};
struct stationinfor{
	my_string<35>stationname;
	trainid ID;
	stationinfor(){}
	stationinfor(my_string<35>& sta, my_string<25>tra, int n){
		stationname = sta;
		ID = trainid(tra,n);
	}
	inline friend bool operator < (const stationinfor & lhs,const stationinfor & rhs){
		if(!(lhs.stationname == rhs.stationname))return lhs.stationname < rhs.stationname;
		return lhs.ID < rhs.ID;
	}
	inline friend bool operator < (const stationinfor & lhs,const my_string<35> & rhs){
		return lhs.stationname < rhs;
	}
	inline friend bool operator < (const my_string<35> & lhs,const stationinfor & rhs){
		return lhs < rhs.stationname;
	}
	inline friend bool operator == (const stationinfor & lhs,const stationinfor & rhs){
		return lhs.stationname == rhs.stationname and lhs.ID == rhs.ID;
	}
	inline friend bool operator == (const stationinfor & lhs,const my_string<35> & rhs){
		return lhs.stationname == rhs;
	}
	inline friend bool operator == (const my_string<35> & lhs,const stationinfor & rhs){
		return lhs == rhs.stationname;
	}
};
BPT<my_string<35>,stationinfor,int>stations("allstation");
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
BPT<dateAndtrainID,dateAndtrainID,seats>allseats("seats");
struct order{
	my_string<10>statue;
	int time=0;
	my_string<25>trainID;
	my_string<35>from;
	my_string<35>to;
	date_time begin;
	date_time end;
	my_date whichday;
	int price=0;
	int num=0;
	order(){}
	order(const std::string& now_statue, std::string& timeStamp, my_string<25>&train, my_string<35>&f, my_string<35>&t, my_date& day, my_time& t1, my_time& t2, int pricetopay, int sum){
		statue = now_statue;
		std::string TimeStamp = timeStamp.substr(1,timeStamp.length()-2);
		time = str_to_int(TimeStamp);
		trainID = train;
		from = f;
		to = t;
		begin = date_time(day, t1);
		end = date_time(day, t2);
		price = pricetopay;
		num = sum;
		whichday = day;
	}
	inline friend bool operator < (const order & lhs,const order & rhs){
		return lhs.time > rhs.time;
	}
	inline friend bool operator == (const order & lhs,const order & rhs){
		return lhs.time == rhs.time;
	}
	inline friend std::ostream& operator << (std::ostream & io, const order& me){
		io << '[' << me.statue << "] " << me.trainID << ' ' << me.from << ' ' << me.begin << " -> " << me.to << ' ' << me.end << ' ' << me.price << ' ' << me.num;
		return io;
	}
};
struct orderinfor{
	my_string<25>userName;
	int time=0;
	orderinfor(){}
	orderinfor(my_string<25>&user, std::string & ts){
		userName = user;
		std::string TimeStamp = ts.substr(1,ts.length()-2);
		time = str_to_int(TimeStamp);
	}
	orderinfor(my_string<25>&user, int ts){
		userName = user;
		time = ts;
	}
	inline friend bool operator < (const orderinfor & lhs,const orderinfor & rhs){
		if(!(lhs.userName == rhs.userName))return lhs.userName < rhs.userName;
		return lhs.time > rhs.time;
	}
	inline friend bool operator < (const orderinfor & lhs,const my_string<25> & rhs){
		return lhs.userName < rhs;
	}
	inline friend bool operator < (const my_string<25> & lhs,const orderinfor & rhs){
		return lhs < rhs.userName;
	}
	inline friend bool operator == (const orderinfor & lhs,const orderinfor & rhs){
		return lhs.time == rhs.time and lhs.userName == rhs.userName;
	}
	inline friend bool operator == (const orderinfor & lhs,const my_string<25> & rhs){
		return lhs.userName == rhs;
	}
	inline friend bool operator == (const my_string<25> & lhs,const orderinfor & rhs){
		return lhs == rhs.userName;
	}
};
struct pendinfor{
	my_string<25>trainID;
	int time=0;
	pendinfor(){}
	pendinfor(my_string<25>&user, std::string & ts){
		trainID = user;
		std::string TimeStamp = ts.substr(1,ts.length()-2);
		time = str_to_int(TimeStamp);
	}
	pendinfor(my_string<25>&user, int ts){
		trainID = user;
		time = ts;
	}
	inline friend bool operator < (const pendinfor & lhs,const pendinfor & rhs){
		if(!(lhs.trainID == rhs.trainID))return lhs.trainID < rhs.trainID;
		return lhs.time < rhs.time;
	}
	inline friend bool operator < (const pendinfor & lhs,const my_string<25> & rhs){
		return lhs.trainID < rhs;
	}
	inline friend bool operator < (const my_string<25> & lhs,const pendinfor & rhs){
		return lhs < rhs.trainID;
	}
	inline friend bool operator == (const pendinfor & lhs,const pendinfor & rhs){
		return lhs.time == rhs.time and lhs.trainID == rhs.trainID;
	}
	inline friend bool operator == (const pendinfor & lhs,const my_string<25> & rhs){
		return lhs.trainID == rhs;
	}
	inline friend bool operator == (const my_string<25> & lhs,const pendinfor & rhs){
		return lhs == rhs.trainID;
	}
};

BPT<my_string<25>, orderinfor, order>orders("orders");
struct pend{
	my_string<25>username;
	int time=0;
	my_string<35>from;
	my_string<35>to;
	date_time begin;
	date_time end;
	my_date whichday;
	int price=0;
	int num=0;
	pend(){}
	pend(std::string& timeStamp, my_string<25>&user, my_string<35>&f, my_string<35>&t, my_date& day, my_time& t1, my_time& t2, int pricetopay, int sum){
		std::string TimeStamp = timeStamp.substr(1,timeStamp.length()-2);
		time = str_to_int(TimeStamp);
		username = user;
		from = f;
		to = t;
		begin = date_time(day, t1);
		end = date_time(day, t2);
		price = pricetopay;
		num = sum;
		whichday = day;
	}
	inline friend bool operator < (const pend & lhs,const pend & rhs){
		return lhs.time < rhs.time;
	}
	inline friend bool operator == (const pend & lhs,const pend & rhs){
		return lhs.time == rhs.time;
	}
};
BPT<my_string<25>, pendinfor, pend>waiting("waiting");
struct trainInforSortbyTime{
	my_string<25>trainID;
	my_string<35>from;
	my_string<35>to;
	date_time begin;
	date_time end;
	int time=0;
	int price=0;
	int num=0;
	trainInforSortbyTime(){}
	trainInforSortbyTime(my_string<25>&train, my_string<35>&f, my_string<35>&t, my_date& day, my_time& t1, my_time& t2, int pricetopay, int sum){
		time = mid(t1,t2);
		trainID = train;
		from = f;
		to = t;
		begin = date_time(day, t1);
		end = date_time(day, t2);
		price = pricetopay;
		num = sum;
	}
	inline friend bool operator < (const trainInforSortbyTime & lhs,const trainInforSortbyTime & rhs){
		if(lhs.time != rhs.time) return lhs.time < rhs.time;
		return lhs.trainID < rhs.trainID;
	}
	inline friend bool operator == (const trainInforSortbyTime & lhs,const trainInforSortbyTime & rhs){
		return lhs.trainID == rhs.trainID;
	}
	inline friend std::ostream& operator << (std::ostream & io, const trainInforSortbyTime& me){
		io << me.trainID << ' ' << me.from << ' ' << me.begin << " -> " << me.to << ' ' << me.end << ' ' << me.price << ' ' << me.num;
		return io;
	}
};
struct trainInforSortbyPrice{
	my_string<25>trainID;
	my_string<35>from;
	my_string<35>to;
	date_time begin;
	date_time end;
	int time=0;
	int price=0;
	int num=0;
	trainInforSortbyPrice(){}
	trainInforSortbyPrice(my_string<25>&train, my_string<35>&f, my_string<35>&t, my_date& day, my_time& t1, my_time& t2, int pricetopay, int sum){
		time = mid(t2,t1);
		trainID = train;
		from = f;
		to = t;
		begin = date_time(day, t1);
		end = date_time(day, t2);
		price = pricetopay;
		num = sum;
	}
	inline friend bool operator < (const trainInforSortbyPrice & lhs,const trainInforSortbyPrice & rhs){
		if(lhs.price != rhs.price)return lhs.price < rhs.price;
		return lhs.trainID < rhs.trainID;
	}
	inline friend bool operator == (const trainInforSortbyPrice & lhs,const trainInforSortbyPrice & rhs){
		return lhs.trainID == rhs.trainID;
	}
	inline friend std::ostream& operator << (std::ostream & io, const trainInforSortbyPrice& me){
		io << me.trainID << ' ' << me.from << ' ' << me.begin << " -> " << me.to << ' ' << me.end << ' ' << me.price << ' ' << me.num;
		return io;
	}
};
struct trainID_seat_price_date_time{
	my_string<25>trainName;
	int seatNum;
	int price;
	my_date date;
	my_time time;
	my_time time_to_start;
	trainID_seat_price_date_time(){}
	trainID_seat_price_date_time(my_string<25>&tr, int s, int p, my_date& da, my_time& ti, my_time& t2){
		trainName = tr;
		seatNum = s;
		price = p;
		date = da;
		time = ti;
		time_to_start = t2;
	}
};
int main(){
//	freopen("../testcases/pressure_3_easy/1.in","r",stdin);
//	freopen("test.txt","w",stdout);
	std::ios::sync_with_stdio(false);
	cin.tie(0),cout.tie(0);
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
						auto ans = loginuser.find(hash(cur_user.ch));
						if(!userbank.empty()){
							if(ans == loginuser.end())flag = false;
							else cgg = loginuser[hash(cur_user.ch)];
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
				if(userbank.Find(hash(user_name.ch)) != nullptr)flag = false;
				if(flag){
					user new_user(pp,nn,mm,gg);
					userbank.Insert(hash(user_name.ch), new_user);
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
				auto ans = userbank.Find(hash(user_name.ch));
				if(ans != nullptr and ans->password == pp and loginuser.find(hash(user_name.ch))==loginuser.end() and flag){
					loginuser[hash(user_name.ch)] = ans->privilege;
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "logout"){
				std::string uu;
				cin >> opt2 >> uu;
				my_string<25>user_name(uu);
				auto iter = loginuser.find(hash(user_name.ch));
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
				if(loginuser.find(hash(cur_name.ch))==loginuser.end())flag= false;
				if(flag){
					auto ans = userbank.Find(hash(user_name.ch));
					if(ans == nullptr)flag = false;
					if(flag){
						int cur_g = loginuser[hash(cur_name.ch)];
						if(ans->privilege < cur_g or cc == uu){
							cout << timestamp << ' ' << uu << ' ' << ans->name << ' ' << ans->mailAddr << ' ' << ans->privilege
							     << '\n';
						}else cout << timestamp << " -1\n";
					}
					else cout << timestamp << " -1\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "modify_profile"){
				std::string line;
				getline(std::cin,line);
				line.append(" ");
				std::string cc,uu,pp,nn,mm;int gg=0;
				bool g = false;
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
							g = true;
							while (line[++i] != ' '){
								gg = gg * 10 + line[i] - '0';
							}
						}
						opt2.clear();
					}
					else{
						opt2 += line[i];
					}
				}
				my_string<25>cur_name(cc),user_name(uu);
				auto ans = userbank.Find(hash(user_name.ch));

				if(loginuser.find(hash(cur_name.ch))!=loginuser.end() and ans!= nullptr){
					int cur_g = userbank.Find(hash(cur_name.ch))->privilege;
					if((cur_g > ans->privilege or cc == uu) and (!g or gg < cur_g)){
						if(!pp.empty())strcpy(ans->password.ch, pp.c_str());
						if(!nn.empty())strcpy(ans->name.ch, nn.c_str());
						if(!mm.empty())strcpy(ans->mailAddr.ch, mm.c_str());
						if(g)ans->privilege = gg;
						if(loginuser.find(hash(user_name.ch)) != loginuser.end())loginuser[hash(user_name.ch)] = ans->privilege;
						cout << timestamp << ' ' << uu << ' ' << ans->name << ' ' << ans->mailAddr << ' ' << ans->privilege
						     << '\n';
					}else cout << timestamp << " -1\n";
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
				if(trainbank.Find(hash(trainID.ch)).empty()){
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
					trainbank.Insert(hash(trainID.ch),new_train);
					cout << timestamp << " 0\n";

				}else cout << timestamp << " -1\n";
			}
			else if(opt == "release_train"){
				cin >> opt2;
				std::string infor;
				cin >> infor;
				my_string<25>trainID(infor);
				auto tr = trainbank.FindExac(hash(trainID.ch));
				if(tr.second and !(tr.first.is_release)){
					train tra = tr.first;
					tra.is_release = true;
					for(int i = 0; i < tra.stationNum; ++i){
						stationinfor si(tra.stations[i],trainID,i);
						stations.Insert(si,1);
					}
					my_date useless = tra.beginSale;
					for(int i = 0; i <= between(tra.beginSale,tra.endSale); ++i){
						dateAndtrainID dat(useless, trainID);
						allseats.Insert(dat, seats(tra.stationNum, tra.seatNum));
						useless.add(1);
					}
					cout << timestamp << " 0\n";
					trainbank.overwrite(hash(trainID.ch), tra);
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
				auto tr = trainbank.FindExac(hash(trainID.ch));
				my_string<35>from(ff);
				my_string<35>to(tt);
				int ind1=-1,ind2=-1;
				if(!(tr.second))flag = false;
				if(loginuser.find(hash(username.ch))!=loginuser.end()){
					train tra = tr.first;
					if(!tra.is_release)flag = false;
					else{
						for(int i = 0; i < tra.stationNum; ++i){
							if(tra.stations[i] == from)ind1 = i;
							if(tra.stations[i] == to)ind2 = i;
						}
						if(ind1 < 0)flag = false;
						if(ind2 < 0)flag = false;
						if(flag and ind1 >= ind2)flag = false;
					}
				}else flag = false;
				if(flag){
					train tra = tr.first;
					my_date dateNeed(dd);
					my_date useless = dateNeed.minus_time(tra.leaveTime[ind1]);
					if(!(tra.beginSale <= useless and useless <= tra.endSale))flag = false;
					int seatNeed = str_to_int(nn);
					if(seatNeed > tra.seatNum)flag = false;
					if(flag){
						dateAndtrainID dat(useless, trainID);
						auto s = allseats.FindExac(dat).first;
						int min_seat = tra.seatNum;
						for(int i = ind1; i < ind2; ++i){
							if(s.seat[i] < min_seat)min_seat = s.seat[i];
						}
						if(min_seat >= seatNeed){
							for(int i = ind1; i < ind2; ++i){
								s.seat[i] -= seatNeed;
							}
							long long priceToPay = 1ll * seatNeed * (tra.prices[ind2] - tra.prices[ind1]);
							cout << timestamp << ' ' << priceToPay << '\n';
							order new_order("success", timestamp, trainID, from, to, useless, tra.leaveTime[ind1], tra.arriveTime[ind2], tra.prices[ind2] - tra.prices[ind1], seatNeed);
							orderinfor od(username, timestamp);
							orders.Insert(od, new_order);
							allseats.overwrite(dat, s);
						}
						else{
							if(q){
								order new_order("pending", timestamp, trainID, from, to, useless, tra.leaveTime[ind1], tra.arriveTime[ind2], tra.prices[ind2] - tra.prices[ind1], seatNeed);
								orderinfor od(username, timestamp);
								pendinfor odd(trainID, timestamp);
								orders.Insert(od, new_order);
								pend new_pend(timestamp, username, from, to, useless, tra.leaveTime[ind1], tra.arriveTime[ind2], tra.prices[ind2] - tra.prices[ind1], seatNeed);
								waiting.Insert(odd, new_pend);
								cout << timestamp << " queue\n";
							}else flag = false;
						}
					}
				}
				if(!flag)cout << timestamp << " -1\n";
			}
			else if(opt == "query_ticket"){
				std::string line;
				getline(std::cin, line);
				line.append(" ");
				std::string ss,tt,dd,pp;
				opt2.clear();
				for(int i = 0; i < line.length(); ++i){
					if(line[i] == ' '){
						if(opt2 == "-d"){
							while (line[++i]!=' '){
								dd += line[i];
							}
						}else if(opt2 == "-s"){
							while (line[++i]!=' '){
								ss += line[i];
							}
						}else if(opt2 == "-t"){
							while (line[++i]!=' '){
								tt += line[i];
							}
						}else if(opt2 == "-p"){
							while (line[++i]!=' '){
								pp += line[i];
							}
						}
						opt2.clear();
					}else opt2 += line[i];
				}
				my_string<35>from(ss),to(tt);
				my_date dateNeed(dd);
				if(pp.empty()) pp = "time";
				if(pp == "time"){
					sjtu::map<trainInforSortbyTime,int>alltrain;
					auto trainlist1 = stations.FindAllKey(from);
					auto trainlist2 = stations.FindAllKey(to);
					int ptr1 = 0, ptr2 = 0;
					while (ptr1 < trainlist1.size() and ptr2 < trainlist2.size()){
						if(trainlist1[ptr1].ID.trainID < trainlist2[ptr2].ID.trainID){
							++ptr1;
							continue;
						}
						else if(trainlist2[ptr2].ID.trainID < trainlist1[ptr1].ID.trainID){
							++ptr2;
							continue;
						}
						else{//find it
							if(trainlist1[ptr1].ID.num >= trainlist2[ptr2].ID.num){
								++ptr1,++ptr2;
								continue;
							}
							trainid id1 = trainlist1[ptr1].ID;
							trainid id2 = trainlist2[ptr2].ID;
							train tr = trainbank.FindExac(hash(id1.trainID.ch)).first;
							bool time_val = false;
							my_date useless = dateNeed.minus_time(tr.leaveTime[id1.num]);
							if(tr.beginSale <= useless and useless <= tr.endSale)time_val = true;
							if(time_val){
								dateAndtrainID dat(useless, id1.trainID);
								seats s = allseats.FindExac(dat).first;
								int min_seat = tr.seatNum;
								for(int i = id1.num; i < id2.num; ++i){
									if(s.seat[i] < min_seat)min_seat = s.seat[i];
								}
								int price = tr.prices[id2.num] - tr.prices[id1.num];
								alltrain[trainInforSortbyTime(id1.trainID,from,to,useless,tr.leaveTime[id1.num],tr.arriveTime[id2.num],price,min_seat)]=1;
							}
							ptr1 ++,ptr2++;
						}
					}
					cout << timestamp << ' ' << alltrain.size() << '\n';
					for(auto i = alltrain.begin(); i != alltrain.end() ; ++i){
						cout << i->first << '\n';
					}
				}else if(pp == "cost"){

					sjtu::map<trainInforSortbyPrice, int> alltrain;
					auto trainlist1 = stations.FindAllKey(from);
					auto trainlist2 = stations.FindAllKey(to);
					int ptr1 = 0, ptr2 = 0;
					while (ptr1 < trainlist1.size() and ptr2 < trainlist2.size()){
						if(trainlist1[ptr1].ID.trainID < trainlist2[ptr2].ID.trainID){
							++ptr1;
							continue;
						}
						else if(trainlist2[ptr2].ID.trainID < trainlist1[ptr1].ID.trainID){
							++ptr2;
							continue;
						}
						else{//find it
							if(trainlist1[ptr1].ID.num >= trainlist2[ptr2].ID.num){
								++ptr1, ++ptr2;
								continue;
							}
							trainid id1 = trainlist1[ptr1].ID;
							trainid id2 = trainlist2[ptr2].ID;
							train tr = trainbank.FindExac(hash(id1.trainID.ch)).first;
							bool time_val = false;
							my_date useless = dateNeed.minus_time(tr.leaveTime[id1.num]);
							if(tr.beginSale <= useless and useless <= tr.endSale)time_val = true;
							if(time_val){
								dateAndtrainID dat(useless, id1.trainID);
								seats s = allseats.FindExac(dat).first;
								int min_seat = tr.seatNum;
								for(int i = id1.num; i < id2.num; ++i){
									if(s.seat[i] < min_seat)min_seat = s.seat[i];
								}
								int price = tr.prices[id2.num] - tr.prices[id1.num];
								alltrain[trainInforSortbyPrice(id1.trainID, from, to, useless, tr.leaveTime[id1.num],
								                               tr.arriveTime[id2.num], price, min_seat)] = 1;
							}
							ptr1 ++,ptr2++;
						}
					}
					cout << timestamp << ' ' << alltrain.size() << '\n';
					for(auto i = alltrain.begin(); i != alltrain.end(); ++i){
						cout << i->first << '\n';
					}

				}

			}
			else if(opt == "query_order"){
				std::string user;
				cin >> opt2 >> user;
				my_string<25>username(user);
				if(loginuser.find(hash(username.ch))!=loginuser.end()){
					sjtu::vector<order>allorders = orders.Find(username);
					cout << timestamp << ' ' << allorders.size() << '\n';
					for(int i=0;i<allorders.size();i++){
						cout << allorders[i] << '\n';
					}
				}else cout << timestamp <<" -1\n";
			}
			else if(opt == "refund_ticket"){
				std::string line;
				getline(std::cin, line);
				line.append(" ");
				std::string uu,nn;
				opt2.clear();
				for(int i = 0; i < line.length(); ++i){
					if(line[i] == ' '){
						if(opt2 == "-u"){
							while (line[++i]!=' '){
								uu += line[i];
							}
						}else if(opt2 == "-n"){
							while (line[++i]!=' '){
								nn += line[i];
							}
						}
						opt2.clear();
					}else opt2 += line[i];
				}
				int n = 1;
				if(!nn.empty())n = str_to_int(nn);
				my_string<25>username(uu);
				if(loginuser.find(hash(username.ch))!=loginuser.end()){
					sjtu::vector<order> allorders = orders.Find(username);
					if(n <= allorders.size()){
						order tar = allorders[n - 1];
						int ind1 = 0, ind2 = 0;
						train tr = trainbank.FindExac(hash(tar.trainID.ch)).first;
						for(int i = 0; i < tr.stationNum; ++i){
							if(tr.stations[i] == tar.from)ind1 = i;
							if(tr.stations[i] == tar.to)ind2 = i;
						}
						if(tar.statue == "success"){
							tar.statue = "refunded";
							orderinfor od(username, tar.time);
							orders.overwrite(od, tar);
							dateAndtrainID dat(tar.whichday, tar.trainID);
							seats s = allseats.FindExac(dat).first;
							for(int i = ind1; i < ind2; ++i){
								s.seat[i] += tar.num;
							}
							sjtu::vector<pend> waitlist = waiting.Find(tar.trainID);
							for(int i = 0; i < waitlist.size(); ++i){
								pend todo = waitlist[i];
								if(todo.whichday == tar.whichday){
									for(int j = 0; j < tr.stationNum; ++j){
										if(tr.stations[j] == todo.from)ind1 = j;
										if(tr.stations[j] == todo.to)ind2 = j;
									}
									int min_seat = tr.seatNum;
									for(int j = ind1; j < ind2; ++j){
										if(s.seat[j] < min_seat)min_seat = s.seat[j];
									}
									if(min_seat >= todo.num){
										my_string<25> waituser = todo.username;
										sjtu::vector<order> new_orderlist = orders.Find(waituser);
										orderinfor od2(waituser, todo.time);
										pendinfor odd2(tar.trainID, todo.time);
										for(int j = 0; j < new_orderlist.size(); ++j){
											if(new_orderlist[j].time == todo.time){
												new_orderlist[j].statue = "success";
												orders.overwrite(od2, new_orderlist[j]);
											}
										}
										for(int j = ind1; j < ind2; ++j){
											s.seat[j] -= todo.num;
										}
										waiting.Delete(odd2, todo);
									}
								}
							}
							allseats.overwrite(dat, s);
							cout << timestamp << " 0\n";
						}
						else if(tar.statue == "pending"){
							orderinfor od(username, tar.time);
							pendinfor od2(tar.trainID, tar.time);
							tar.statue = "refunded";
							orders.overwrite(od, tar);
							sjtu::vector<pend> waitlist = waiting.Find(tar.trainID);
							for(int i = 0; i < waitlist.size(); ++i){
								if(waitlist[i].time == tar.time){
									waiting.Delete(od2, waitlist[i]);
									break;
								}
							}
							cout << timestamp << " 0\n";
						}
						else if(tar.statue == "refunded"){
							cout << timestamp << " -1\n";
						}
					}
					else cout << timestamp << " -1\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "query_train"){
				std::string ii,dd;
				for(int i = 0; i < 2; ++i){
					cin >> opt2;
					if (opt2 == "-i"){
						cin >> ii;
					}
					else if(opt2 == "-d"){
						cin >> dd;
					}
				}
				my_string<25>trainID(ii);
				my_date dateNeed(dd);
				auto tr = trainbank.FindExac(hash(trainID.ch));
				if(tr.second and tr.first.beginSale <= dateNeed and dateNeed <= tr.first.endSale){
					dateAndtrainID dat(dateNeed, trainID);
					auto s = allseats.FindExac(dat);
					train tra = tr.first;
					cout << timestamp << ' ' << trainID << ' ' << tra.type << '\n';
					for(int i = 0; i < tra.stationNum; ++i){
						cout << tra.stations[i] << ' ' << date_time(dateNeed , tra.arriveTime[i]) << " -> " << date_time(dateNeed, tra.leaveTime[i]) << ' ' << tra.prices[i] << ' ';
						if(i != tra.stationNum-1){
							if(s.second)cout << s.first.seat[i] << '\n';
							else cout << tra.seatNum << '\n';
						}
						else cout << "x\n";
					}
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "query_transfer"){
				std::string line;
				getline(std::cin, line);
				line.append(" ");
				std::string ss,tt,dd,pp;
				opt2.clear();
				for(int i = 0; i < line.length(); ++i){
					if(line[i] == ' '){
						if(opt2 == "-d"){
							while (line[++i]!=' '){
								dd += line[i];
							}
						}else if(opt2 == "-s"){
							while (line[++i]!=' '){
								ss += line[i];
							}
						}else if(opt2 == "-t"){
							while (line[++i]!=' '){
								tt += line[i];
							}
						}else if(opt2 == "-p"){
							while (line[++i]!=' '){
								pp += line[i];
							}
						}
						opt2.clear();
					}else opt2 += line[i];
				}
				my_string<35>from(ss),to(tt);
				std::string p = "time";
				if(!pp.empty())p = pp;
				my_date dateNeed(dd);
				auto alltrianfrom = stations.FindAllKey(from);
				auto alltrainto = stations.FindAllKey(to);
				if(alltrianfrom.empty() or alltrainto.empty()){
					cout << timestamp << " 0\n";
					continue;
				}
				sjtu::map<my_string<35>, sjtu::vector<trainID_seat_price_date_time>>me;
				for(int i = 0; i < alltrianfrom.size(); ++i){
					stationinfor atf = alltrianfrom[i];
					train tr = trainbank.FindExac(hash(atf.ID.trainID.ch)).first;
					my_date date1 = dateNeed.minus_time(tr.leaveTime[atf.ID.num]);
					if(!(tr.beginSale <= date1 and date1 <= tr.endSale))continue;
					seats s = allseats.FindExac(dateAndtrainID(date1, atf.ID.trainID)).first;
					int seatNum = tr.seatNum;
					for(int j = atf.ID.num + 1; j < tr.stationNum; ++j){
						seatNum = std::min(seatNum, s.seat[j-1]);
						me[tr.stations[j]].push_back(trainID_seat_price_date_time(atf.ID.trainID,seatNum,tr.prices[j] - tr.prices[atf.ID.num], date1,tr.arriveTime[j], tr.leaveTime[atf.ID.num]));
					}
				}
				bool is_ans = false;
				int ans_time=0,ans_cost=0;
				trainInforSortbyTime ans1{};
				trainInforSortbyTime ans2{};
				for(int i = 0; i < alltrainto.size(); ++i){
					stationinfor atf = alltrainto[i];
					train tr2 = trainbank.FindExac(hash(atf.ID.trainID.ch)).first;
					for(int j = 0; j < atf.ID.num; j++){
						auto iter = me.find(tr2.stations[j]);
						if(iter == me.end())continue;
						auto firsttrains = iter->second;
						for(int k = 0; k < firsttrains.size(); ++k){
							trainID_seat_price_date_time* tspdt = firsttrains.head[k];
							if(!(tspdt->trainName == atf.ID.trainID)){
								my_date date2 = tspdt->date.add_time(tspdt->time).minus_time(tr2.leaveTime[j]);
								if(compare(tspdt->time, tr2.leaveTime[j]))date2.add(1);
								if(date2 < tr2.endSale){
									if(date2 < tr2.beginSale)date2 = tr2.beginSale;
									seats s2 = allseats.FindExac(dateAndtrainID(date2, atf.ID.trainID)).first;
									int seatNum = tr2.seatNum;
									for(int l = k; l < atf.ID.num; ++l){
										seatNum = std::min(seatNum, s2.seat[l]);
									}
									int key_time = 0, key_cost = 0;
									key_time = between(tspdt->date,date2) * 24 * 60 + mid(tspdt->time_to_start,tr2.arriveTime[atf.ID.num]);
									key_cost = tspdt->price + tr2.prices[atf.ID.num] - tr2.prices[j];
									if(is_ans){
										if(p == "time"){
											bool flag = false;
											if(key_time != ans_time)flag = key_time < ans_time;
											else if(key_cost != ans_cost)flag = key_cost < ans_cost;
											else if(!(tspdt->trainName == ans1.trainID))
												flag = tspdt->trainName < ans1.trainID;
											else flag = atf.ID.trainID < ans2.trainID;
											if(flag){
												ans_cost = key_cost;
												ans_time = key_time;
												ans1 = trainInforSortbyTime(tspdt->trainName, from,
												                            tr2.stations[j], tspdt->date,
												                            tspdt->time_to_start,
												                            tspdt->time, tspdt->price,
												                            tspdt->seatNum);
												ans2 = trainInforSortbyTime(atf.ID.trainID,
												                            tr2.stations[j], to, date2,
												                            tr2.leaveTime[j],
												                            tr2.arriveTime[atf.ID.num], tr2.prices[atf.ID.num] -
												                                               tr2.prices[j],
												                            seatNum);
											}
										}
										else if(p == "cost"){
											bool flag = false;
											if(key_cost != ans_cost)flag = key_cost < ans_cost;
											else if(key_time != ans_time)flag = key_time < ans_time;
											else if(!(tspdt->trainName == ans1.trainID))
												flag = tspdt->trainName < ans1.trainID;
											else flag = atf.ID.trainID < ans2.trainID;
											if(flag){
												ans_cost = key_cost;
												ans_time = key_time;
												ans1 = trainInforSortbyTime(tspdt->trainName, from,
												                            tr2.stations[j], tspdt->date,
												                            tspdt->time_to_start,
												                            tspdt->time, tspdt->price,
												                            tspdt->seatNum);
												ans2 = trainInforSortbyTime(atf.ID.trainID,
												                            tr2.stations[j], to, date2,
												                            tr2.leaveTime[j],
												                            tr2.arriveTime[atf.ID.num], tr2.prices[atf.ID.num] -
												                                                        tr2.prices[j],
												                            seatNum);
											}
										}
									}
									else{
										is_ans = true;
										ans_time = key_time;
										ans_cost = key_cost;
										ans1 = trainInforSortbyTime(tspdt->trainName, from,
										                            tr2.stations[j], tspdt->date,
										                            tspdt->time_to_start,
										                            tspdt->time, tspdt->price,
										                            tspdt->seatNum);
										ans2 = trainInforSortbyTime(atf.ID.trainID,
										                            tr2.stations[j], to, date2,
										                            tr2.leaveTime[j],
										                            tr2.arriveTime[atf.ID.num], tr2.prices[atf.ID.num] -
										                                                        tr2.prices[j],
										                            seatNum);
									}
								}
							}
						}
					}
				}
				if(is_ans){
					cout << timestamp << ' ';
					cout << ans1 << '\n' << ans2 << '\n';
				}else cout << timestamp << " 0\n";
			}
			else if(opt == "delete_train"){
				std::string ii;
				cin >> opt2 >> ii;
				my_string<25>trainID(ii);
				train tr = trainbank.FindExac(hash(trainID.ch)).first;
				if(!tr.is_release){
					trainbank.Delete(hash(trainID.ch), tr);
					cout << timestamp << " 0\n";
				}else cout << timestamp << " -1\n";
			}
			else if(opt == "exit"){
				cout << timestamp << " bye\n";
				break;
			}else return -1;
		}
	}
	return 0;
} 