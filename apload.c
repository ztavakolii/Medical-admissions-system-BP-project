#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <wchar.h>
#pragma warning(disable:4996)
#define hit 250

struct Noskhe {
	char noskhe[300];
};

struct turn {
	bool reservedorno;
	int turnpayment;
	int patientID;
	int doctorID;
	char doctorname[31];
	char expertise[21];
	char patienrname[31];
	char hour[6];
};

struct patient_turn {
	int turn_date;
	int doctor_ID;
};

struct patient {
	char name[31];
	char meli[11];
	char password[7];
	char email[34];
	int ID;
	char wallet[11];
	Noskhe noskhearr[30];
	int numnoskhe;
	patient_turn turnsinfo[50];// ارایه ای برای نوبت های گرفته شده بیمار در ماه
	int numturns;
}p, tempp;

struct doctor {
	char name[31];
	char expertise[21];
	char meli[11];
	char password[7];
	char email[34];
	int ID;
	unsigned long int wallet = 10;
	int visirpayment;
	int visittime;
	bool extension;
}d, temp;

struct shift {
	int begintime;//the hour of begining doctor`s shift time;
	int endtime;// the hour of finishing doctor`s shift time;
	turn arrturns[156];
};

struct date {
	int _date;//Which date of the month
	int holiday;
	char reason[51];//reason of closed
	int beginworktime;
	int endworktime;// worktime of complex
	shift doctorsshift[50];
};

enum weekday { sunday, monday, tuesday, wednesday, thursday, friday, saturday };

struct Rent {
	int numberd = 0;// the number of doctors in complex
	int numberp = 0;//the number of patients in complex
	bool notpaymentthismonth[50] = { 0 };
	int notpaymentmonths[50] = { 0 };
};
struct TIME { // the struct of things that change by time
	Rent rent; // this struct next each change must save in timefile
	tm infopreentry;
}T;

int managetime(tm* curentry);
void Kbhit(const char* s, int column);
int Numdays();
int mainpage();
int signin();
void forgetpassword();
void Secretaryaccount();
void adddoctor();
void addpatient();
void monthlyschedule();
void setcalendar(int whatmonth, int view, int);
void viewcalendar(int whatmonth, int num);
void visitsschedule();
void choosedoctor();
void all();
void doctoraccount(int ID);
void Determiningshifts(int ID);
int numberturns(char* hourshift, int visittime);
char* hourvisit(char* start, int visittime, int loopindex);
void rentpayment(int);
void patientprescription(int ID);
void patientaccount(int ID);
void bookappointment(int ID);
void reservedvisits(int ID);
void prescriptions(int ID);
void allappointments(int ID);
void cancelappointment(int ID);
void listpatients();

time_t t;   char week[11];
int mod, numcur, numnext;//num is the number of days in current month
char res[51];
date mon[42];// ban is a array in heap to save turns
tm* curentry, * preentry;
char username[11], password[7], email[51], ch, tok[6], tem[6], srr[51], q[7], temppp[300] = "", www[11];
int pay, l, count, line, teme = 20;

int main() {
	_setmode(_fileno(stdout), _O_U8TEXT);
	system("color F0");
	wprintf(L"\033[?25l");

	FILE* ind = fopen("doctorsfile.bin", "ab");
	FILE* inp = fopen("patientsfile.bin", "ab");
	FILE* tf = fopen("timefile.bin", "ab");
	FILE* currentcal = fopen("current month.bin", "ab");
	FILE* nextcal = fopen("next month.bin", "ab");
	FILE* o = fopen("temefile.bin", "ab+");
	if (ind == NULL || inp == NULL || tf == NULL || currentcal == NULL || nextcal == NULL || o == NULL)
		return -1;
	d = { "","","","","",0,10,0,0,0 };
	fwrite(&d, sizeof(doctor), 50, ind);
	p = { "","","","",0,"", {{0}},0,{ {0,0}},0 };
	fwrite(&p, sizeof(patient), 800, inp);
	if (fread(&teme, sizeof(int), 1, o));
	else teme = 20;
	fcloseall();
	t = time(NULL);  curentry = localtime(&t);//curentry pointer to current system time
line0:	if (curentry->tm_mon != (T.infopreentry).tm_mon) {
	int a = managetime(curentry);// تابعی که تغییرات گذشت زمان را انجام میدهد فراخوانی کن
	if (a == -1)
		goto line0;
}
int a = mainpage();
if (a == 1) {
	return 0;
}
}

int managetime(tm* curentry) {
	FILE* tf = fopen("timefile.bin", "rb+");
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* inp = fopen("patientsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "wb");
	FILE* nextcal = fopen("next month.bin", "rb+");
	fread(&T, sizeof(TIME), 1, tf);
	d = { "","","","","",0,10,0,0,0 };
	if (tf == NULL || ind == NULL) {
		fclose(ind);
		fclose(tf);
		return -1;
	}
	for (int i = 0; i < 50; i++) {
		if (T.rent.notpaymentthismonth[i] == 0) {
			T.rent.notpaymentmonths[i] += curentry->tm_mon - (T.infopreentry).tm_mon;
		}
		else if (T.rent.notpaymentthismonth[i] == 1) {
			T.rent.notpaymentmonths[i] += curentry->tm_mon - (T.infopreentry).tm_mon - 1;
		}
		T.rent.notpaymentthismonth[i] = 0;
	}
	for (int i = 0; i < 50; i++) {
		if (T.rent.notpaymentmonths[i] >= 2) {
			fseek(ind, i * sizeof(doctor), SEEK_SET);
			fwrite(&d, sizeof(doctor), 1, ind);
			T.rent.numberd--;
			T.rent.notpaymentmonths[i] = 0;
		}
	}
	T.infopreentry = *(curentry);
	rewind(tf);
	fwrite(&T, sizeof(TIME), 1, tf);
	fclose(ind);
	fclose(tf);
	char str[16];
	fread(str, sizeof str, 1, nextcal);
	fread(mon, sizeof mon, 1, nextcal);
	fwrite(str, sizeof str, 1, currentcal);
	fwrite(mon, sizeof mon, 1, currentcal);
	setcalendar(1, 0, 0);
	while (fread(&p, sizeof(patient), 1, inp)) {
		p.numnoskhe = 0;
		p.numturns = 0;
		for (int i = 0; i < 30; i++)
			p.noskhearr[i] = { {0} };
		for (int i = 0; i < 50; i++)
			p.turnsinfo[i] = { {0} };
	}
	fcloseall();
	return 1;
}
void Kbhit(const char* s, int column) {
	wprintf(L"\033[2;%dH\033[?25l", column);
	while (!kbhit()) {
		wprintf(L"\x1b[38;5;%dm\x25CF %S  ",teme, s);
		Sleep(hit);
		wprintf(L"\033[2;%dH\033[?25l", column);
		wprintf(L"\x1b[38;5;%dm  %S \x25CF",teme, s);
		Sleep(hit);
		wprintf(L"\033[2;%dH\033[?25l", column);
	}
}
int Numdays() {
	int numdays;
	switch (curentry->tm_mon) {
	case 0:
		numdays = 31;
		break;
	case 1:
		if ((curentry->tm_year + 1900) % 100 == 0 && (curentry->tm_year + 1900) % 400 == 0)
			numdays = 29;
		else if ((curentry->tm_year + 1900) % 4 == 0 && (curentry->tm_year + 1900) % 100 != 0)
			numdays = 29;
		else numdays = 28;
		break;
	case 2: numdays = 31;   break;
	case 3: numdays = 30;  break;
	case 4: numdays = 31;   break;
	case 5: numdays = 30;  break;
	case 6: numdays = 31;  break;
	case 7: numdays = 31;  break;
	case 8: numdays = 30;  break;
	case 9: numdays = 31;  break;
	case 10: numdays = 30;  break;
	case 11: numdays = 31;  break;
	}
	return numdays;
}

int mainpage() {
	FILE* o;
	_setmode(_fileno(stdout), _O_U8TEXT);
	system("cls");
	system("color F0");
	wprintf(L"\033[?25l");
	wprintf(L"\n \x1b[38;5;232mWelcome to the ");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m1-Sign in\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m2-Forgot password\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m3-The rules of the complex\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m4-Change the theme\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m5-Exit\n \x1b[38;5;232mPlease enter the number of an option:\n ",teme,teme,teme,teme,teme);
	const char* s = "Medical admissions system";    const char* ss = "Change teme";
	char che;
	Kbhit(s, 17);
line1:	wprintf(L"\033[10;2H");
	ch = getche();
	switch (ch) {
	case '1':
		signin();
		return 0;
	case '2':
		forgetpassword();
		return 0;
	case'3':
		system("cls");   system("color F0");
		wprintf(L"\n\n \x1b[38;5;%dmIn the name of God\n\n ", teme);
		wprintf(L"\x1b[38;5;%dmSecretary account features and rules:\n\n"
			L"\x1b[38;5;232m  To add a person(doctor or patient) to the system, use the Add Doctor or Add Patient sections.\n"
			L"  In the Add Patient section, it is possible to view patient information.The maximum number of\n"
			L"  doctors in the complex is 50 and the maximum number of patients is 800, and the system does \n"
			L"  not allow adding more people.\n"
			L"  Use the Monthly schedule section to view, set and change the integrated calendar.Note that in\n"
			L"  order to change the status of a day off, you must act two days in advance.\n"
			L"  Use the Visits Schedule section to view the booked appointments of doctors.In this section, \n"
			L"  using the All section, all booked appointments of all doctors are displayed.Use the Choose Doctor\n"
			L"  section to view the reserved appointments of a specific doctor.\n\n\n"
			L"\x1b[38;5;%dm Doctor account featuresand rules:\x1b[38;5;232m\n\n"
			L"  After adding you to the system by the secretary, you must enter the Forgot password sectionand\n"
			L"  enter your desired password by entering your username(national code) and your email.After that,\n"
			L"  you can use the Sign in section to enter your account.\n"
			L"  In the Determining the shifts section, you can determine the times of your presence in the office\n"
			L"  for patient visits.\n"
			L"  In the Reserved Visits section, you can view your reserved appointments in that time period by \n"
			L"  specifying a time period.\n"
			L"  In the Patient Prescription section, by selecting a patient's ID, you can prescribe a prescription\n"
			L"  for him. Please note that the fee for the patient's visit will be deposited into your wallet only \n"
			L"  after writing a prescription for him.\n"
			L"  Note that you have until the last day of each month to enter the Rent Payment sectionand choose one \n"
			L"  of the options.If you do not do this in one month, one unit will be added to the number of months you\n"
			L"  have not paid the office rent, and if you do not pay the office rent for two months, you will be removed\n"
			L"  from the system.\n"
			L"  Office rent is $10 per day.If you are unable to pay the office rent this month, use the Extension section.\n"
			L"  By choosing this option, next month instead of 10 dollars per day, you will pay 20 dollars per day.Note \n"
			L"  that you are only allowed to use the Extension once every two months, otherwise you will be automatically\n"
			L"  removed from the system.If you have the financial ability to pay the rent, use the Payment section.\n"
			L"  In the Visirs Payment section, you can specify the durationand cost of your visits.The cost of the \n"
			L"  visit should not be less than 10 dollars and more than 50 dollars.\n\n\n"
			L" \x1b[38;5;%dmPatient account features and rules:\x1b[38;5;232m\n\n"
			L"  After adding you to the system by the secretary, you must enter the Forgot password sectionand enter \n"
			L"  your desired password by entering your username(national code) and your email.After that, you can use \n"
			L"  the Sign in section to enter your account.\n"
			L"  Use the Book an Appointment section to book an appointment.The cost of the appointment includes the cost\n"
			L"  of the doctor's visit plus 10%% tax.\n"
			L"  To cancel an appointment, use the Cancel an Appointment section.Note that in case of cancellation, 50%% of\n"
			L"  the payment will be returned to your wallet.\n"
			L"  You can see your booked appointments in the All Appointments section.\n"
			L"  In the Prescriptions section, you can see the prescriptions related to the appointments taken this month.\n"
			L"  Note that upon entering the next month,the prescriptions of the previous month will no longer be visible.",teme,teme,teme);
		wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPress a key to exit.\n", teme);
		getchar();
		if (!kbhit())
			mainpage();
		return 0;
	case '4':
		o = fopen("temefile.bin", "wb");
		if (o == NULL)
			exit(0);
		system("cls");   system("color F0");
		wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease choose your desired theme.\n\n 1-Blue\n 2-Green\n 3-Red\n 4-Orange\n 5-Brown\n 6-Purple\n 7-Pink\n 8-Cyan", teme);
		Kbhit(ss, 2); 
	line81:wprintf(L"\033[14;2H\033[?25l");
		che = getche();
		switch (che) {
		case '1':
			teme = 20;
			break;
		case'2':
			teme = 28;
			break;
		case '3':
			teme = 124;
			break;
		case '4':
			teme = 202;
			break;
		case '5':
			teme = 173;
			break;
		case '6':
			teme = 128;
			break;
		case '7':
			teme = 200;
			break;
		case '8':
			teme = 36;
			break;
		default:
			wprintf(L"\n \a\x1b[38;5;160mPlease enter a number from 1 to 8:\n\x1b[38;5;232m ");
			//getchar();
			Kbhit(ss, 2);
			goto line81;
		}
		fwrite(&teme, sizeof(int), 1, o);
		fclose(o);
		mainpage();
		return 0;
	case '5':
		return 1;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter a number from 1 to 3:\n\x1b[38;5;232m ");
		//getchar();
		Kbhit(s, 17);
		goto line1;
	}
}
int signin() {
	FILE* ind = fopen("doctorsfile.bin", "rb");
	FILE* inp = fopen("patientsfile.bin", "rb");
	/*char username[11], password[7], ch;*/

	int i;    count = 0;
	wprintf(L"\033[?25h");   //*****making curser visible*****
line3:system("cls");
	system("color F0");
	wprintf(L"\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter your username(national code) and password:\n\n \x1b[38;5;%dmUsername:\n [\x1b[38;5;232m            \x1b[38;5;%dm]\n \x1b[38;5;%dmPassword:\n [\x1b[38;5;232m            \x1b[38;5;%dm]",teme,teme,teme,teme,teme);
	wprintf(L"\033[2A");
	wprintf(L"\033[11D");
	wprintf(L"\033[D\x1b[38;5;232m");
	for (i = 0; i < 10 && count != 5; i++) {
		ch = _getche();
		if (ch == '\b') {
			i -= 2;
			continue;
		}
		username[i] = ch;
		switch (ch) {
		case 'A':
			if (i == 0)
				count++;
			break;
		case 'd':
			if (i == 1)
				count++;
			break;
		case 'm':
			if (i == 2)
				count++;
			break;
		case 'i':
			if (i == 3)
				count++;
			break;
		case 'n':
			if (i == 4)
				count++;
		}
	}
	username[i] = '\0';
	count = 0;
	if (strcmp(username, "Admin") == 0) {
		wprintf(L"\033[2B");
		wprintf(L"\033[3D");
		for (i = 0; i < 5; i++) {
			ch = _getche();
			if (ch == '\b') {
				i -= 2;
				continue;
			}
			password[i] = ch;
			wprintf(L"\033[D");
			wprintf(L"*");
		}
		password[i] = '\0';
		if (strcmp(password, "Admin") == 0) {
			//getchar();
			Secretaryaccount();
			fclose(ind);
			fclose(inp);
			return 1;
		}
		else {
			wprintf(L"\n \a\x1b[38;5;160mThe username or password is incorrect.");
			Sleep(2000);
			goto line3;
		}
	}
	else {
		wprintf(L"\033[2B");
		wprintf(L"\033[8D");
		for (i = 0; i < 6; i++) {
			ch = _getche();
			if (ch == '\b') {
				i -= 2;
				continue;
			}
			password[i] = ch;
			wprintf(L"\033[D");
			wprintf(L"*");
		}
		password[i] = '\0';
		rewind(ind);
		while (!feof(ind)) {
			int a = fread(&d, sizeof(doctor), 1, ind);
			if (strcmp(d.meli, username) == 0) {
				if (strcmp(d.password, password) == 0) {
					doctoraccount(d.ID); //ورود به اکانت ان پزشک
					fclose(ind);
					fclose(inp);
					return 1;
				}
				else {
					wprintf(L"\n \a\x1b[38;5;160mThe username or password is incorrect.");
					Sleep(2000);
					goto line3;
				}
			}
		}
		rewind(inp);
		while (!feof(inp)) {
			fread(&p, sizeof(patient), 1, inp);
			if (strcmp(username, p.meli) == 0) {
				if (strcmp(password, p.password) == 0) {
					patientaccount(p.ID); //ورود به اکانت ان بیمار
					fclose(inp);
					fclose(ind);
					return 1;
				}
				else {
					wprintf(L"\n \a\x1b[38;5;160mThe username or password is incorrect.");
					Sleep(2000);
					goto line3;
				}
			}
		}
		wprintf(L"\n \x1b[38;5;160mUser not found, first register in the system.");
		Sleep(2000);
		fclose(ind);
		fclose(inp);
		return 1;
	}
}
void forgetpassword() {
	wprintf(L"\033[?25h");
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* inp = fopen("patientsfile.bin", "rb+");
	/*char username[11], email[51], password[7], ch;*/
	int i;
	p = { "","","","",0,"" ,{{0}},0,{{0,0}},0 };    tempp = { "","","","",0,"" ,{{0}},0,{{0,0}},0 };     d = { "","","","","",0,10,0,0,0 };    temp = { "","","","","",0,10,0,0,0 };
	system("cls");
	system("color F0");
	wprintf(L"\033[?25l");
	wprintf(L"\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter your username(national code) and email:\n\n \x1b[38;5;%dmUsername:\n [\x1b[38;5;232m                                   \x1b[38;5;%dm]\n \x1b[38;5;%dmEmail:\n [\x1b[38;5;232m                                   \x1b[38;5;%dm]",teme,teme,teme,teme,teme);
	wprintf(L"\033[2A");
	for (int j = 0; j < 24; j++) {
		wprintf(L"\033[D\x1b[38;5;232m");
	}
	for (i = 0; i < 10; i++) {
		ch = _getche();
		if (ch == '\b') {
			i -= 2;
			continue;
		}
		username[i] = ch;
	}
	username[i] = '\0';

	wprintf(L"\033[B");
	wprintf(L"\033[B");
	for (int j = 0; j < 21; j++) {
		wprintf(L"\033[D");
	}
	for (i = 0; i < 33; i++) {
		ch = _getche();
		if (ch == '\r') {
			break;
		}
		if (ch == '\b') {
			i -= 2;
			continue;
		}
		email[i] = ch;
	}
	email[i] = '\0';
	rewind(ind);
	while (!feof(ind)) {
		fread(&d, sizeof(doctor), 1, ind);
		if (strcmp(d.meli, username) == 0) {
			if (strcmp(d.email, email) == 0) {
				wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter your new password:\n\n \x1b[38;5;%dmPassword:\n [\x1b[38;5;232m                                   \x1b[38;5;%dm]\x1b[38;5;232m",teme,teme,teme);
				wprintf(L"\033[25D");
				for (i = 0; i < 6; i++) {
					ch = _getche();
					if (ch == '\b') {
						i -= 2;
						continue;
					}
					password[i] = ch;
					wprintf(L"\033[D");
					wprintf(L"*");
				}
				password[i] = '\0';
				rewind(ind);
				while (!feof(ind)) {
					fread(&d, sizeof(doctor), 1, ind);
					if (strcmp(username, d.meli) == 0) {
						char* q = strcpy(d.password, password);
						int a = fseek(ind, (d.ID) * sizeof(doctor), SEEK_SET);
						fwrite(&d, sizeof(doctor), 1, ind);
						break;
					}
				}    // اطلاعات و پسورد جدید را وارد فایل مربوطه کن 
				doctoraccount(d.ID);// وارد اکانت فرد شو
				fclose(ind);
				fclose(inp);
				return;
			}
		}
	}
	rewind(inp);
	while (!feof(inp)) {
		fread(&p, sizeof(patient), 1, inp);
		if (strcmp(p.meli, username) == 0) {
			if (strcmp(p.email, email) == 0) {
				wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter your new password:\n\n \x1b[38;5;%dmPassword:\n [\x1b[38;5;232m                                   \x1b[38;5;%dm]\x1b[38;5;232m",teme,teme,teme);
				wprintf(L"\033[25D");
				for (i = 0; i < 6; i++) {
					ch = _getche();
					if (ch == '\b') {
						i -= 2;
						continue;
					}
					password[i] = ch;
					wprintf(L"\033[D");
					wprintf(L"*");
				}
				rewind(inp);
				while (!feof(inp)) {
					fread(&p, sizeof(patient), 1, inp);
					if (strcmp(username, p.meli) == 0) {
						strcpy(p.password, password);
						fseek(inp, (p.ID) * sizeof(patient), SEEK_SET);
						fwrite(&p, sizeof(patient), 1, inp);
						break;
					}
				}    // اطلاعات و پسورد جدید را وارد فایل مربوطه کن مهم مهم
				patientaccount(p.ID);  // وارد اکانت فرد شو
				fclose(inp);
				fclose(ind);
				return;
			}
		}
	}
	wprintf(L"\n\n \a\x1b[38;5;160mThe username and email do not match.");
	Sleep(2000);
	getchar();
	mainpage();
	fclose(inp);
	fclose(ind);
	return;
}

void Secretaryaccount() {
	/*char ch;*/
	system("cls");
	system("color F0");
	wprintf(L"\033[?25l");
	wprintf(L"\n \x1b[38;5;232mWelcome to ");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m1-Add Doctor\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m2-Add Patient\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m3-Monthly Schedule\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m4-Visits Schedule\n \x1b[38;5;%dm\x0004 \x1b[38;5;232m5-Exit\n \x1b[38;5;%dmEnter the desired option(1/2/3/4/5):\n \x1b[38;5;232m",teme,teme,teme,teme,teme,teme);
	const char* s = "your account";
	Kbhit(s, 13);
line3:	wprintf(L"\033[10;2H");
	ch = getche();
	switch (ch) {
	case '1':
		adddoctor();
		Secretaryaccount();
		return;
	case '2':
		addpatient();
		Secretaryaccount();
		return;
	case'3':
		monthlyschedule();
		Secretaryaccount();
		return;
	case '4':
		visitsschedule();
		Secretaryaccount();
		return;
	case '5':
		//getchar();
		mainpage();
		return;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter a number from 1 to 5:\n\x1b[38;5;232m ");
		Kbhit(s, 13);
		goto line3;
	}
}
void adddoctor() {
	//T.rent.number = 50;
	int countenter = 0, k, j;
	d = { "","","","","",0,10,0,0,0 };      temp = { "","","","","",0,10,0,0,0 };
	FILE* ind = fopen("doctorsfile.bin", "rb+");
line4:	system("cls"); system("color F0");
	wprintf(L"\033[?25h");
	if (T.rent.numberd >= 50) {
		wprintf(L"\n\n\n\n\a \x1b[38;5;160mAll the offices are already rented.\n \x1b[38;5;232m");
		wprintf(L"\033[2;2H");
		for (int i = 0; i < 3; i++) {
			wprintf(L"\x1b[38;5;%dm\x25CF Add doctor  ",teme);
			Sleep(500);
			wprintf(L"\033[2;2H");
			wprintf(L"  Add doctor \x25CF");
			Sleep(500);
			wprintf(L"\033[2;2H");
		}
		fclose(ind);
		return;
	}
	else {
		const char* s = "Add doctor";
		wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease enter doctor's name and expertise and username (national code) and email:\n\n",teme);
		wprintf(L" \x1b[38;5;%dmName:\n [                                   ]\n Expertise:\n [                                   ]\n Username:\n [                                   ]\n Email:\n [                                   ]\x1b[38;5;232m",teme);
		//wprintf(L"\033[8;4H"); //Sleep(7000);
		for (k = 0, j = 4; k < 33; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[7;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			temp.name[k] = ch;
		}
		temp.name[k] = '\0';
		while (!feof(ind)) {
			fread(&d, sizeof(doctor), 1, ind);
			if (strcmp(d.name, temp.name) == 0) {
				wprintf(L"\n\n\n\n\n\n\n\n \a\x1b[38;5;160mThis name is available. Please enter another name.\x1b[38;5;232m");
				Kbhit(s, 2);
				_getch();
				//Sleep(5000);
				fclose(ind);
				return;
			}
		}
		wprintf(L"\033[2B");
		wprintf(L"\033[3C");
		for (k = 0, j = 4; k < 33; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[9;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			temp.expertise[k] = ch;
		}
		temp.expertise[k] = '\0';
		wprintf(L"\033[2B");
		wprintf(L"\033[3C");
		for (k = 0, j = 4; k < 10; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[11;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			temp.meli[k] = ch;
		}
		temp.meli[k] = '\0';
		wprintf(L"\033[2B");
		wprintf(L"\033[10D");

		for (k = 0, j = 4; k < 33; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[13;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			temp.email[k] = ch;
		}
		temp.email[k] = '\0';
		rewind(ind);
		int i = 0;
		while (!feof(ind)) {
			fread(&d, sizeof(doctor), 1, ind);
			if (strcmp(d.name, "") == 0) {
				temp.ID = i;
				break;
			}
			i++;
		}
		fseek(ind, temp.ID * sizeof(doctor), SEEK_SET);
		fwrite(&temp, sizeof(doctor), 1, ind);
		T.rent.numberd++;
		wprintf(L"\n\n \x1b[38;5;160mThe desired person has been added successfully.");
		Kbhit(s, 2);
		_getch();
		//Sleep(4000);
		fclose(ind);
	}
}
void addpatient() {
	FILE* inp = fopen("patientsfile.bin", "rb+");
	int k, j, i = 0;
	p = { "","","","",0,"" ,{{0}},0,{{0}},0 };       tempp = { "","","","",0,"" ,{{0}},0,{{0}},0 };
	system("cls");  system("color F0");
	const char* s = "Add patient";
	wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease choose an option:\n\n 1- Add patient\n 2- View list of patients\n ",teme);
	Kbhit(s, 2);
line4:	wprintf(L"\033[8;2H");
	ch = getche();
	switch (ch) {
	case '1':
		//T.rent.numberp = 800;
		if (T.rent.numberp >= 800) {
			system("cls"); system("color F0");
			wprintf(L"\n\n\n \a\x1b[38;5;160mThe admission capacity for patients has been completed.\x1b[38;5;232m");
			Kbhit(s, 2);
			getch();
			fclose(inp);
			return;
		}
		system("cls"); system("color F0");
		wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease enter the details of the desired person:\n\n",teme);
		wprintf(L" \x1b[38;5;%dmName:\n [                                   ]\n Username(national code):\n [                                   ]\n Email:\n [                                   ]",teme);
		wprintf(L"\n Wallet balance:\n [                                   ]\x1b[38;5;232m");
		for (k = 0, j = 4; k < 33; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[7;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			tempp.name[k] = ch;
		}
		tempp.name[k] = '\0';
		for (k = 0, j = 4; k < 10; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[9;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			tempp.meli[k] = ch;
		}
		tempp.meli[k] = '\0';

		for (k = 0, j = 4; k < 33; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[11;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			tempp.email[k] = ch;
		}
		tempp.email[k] = '\0';

		for (k = 0, j = 4; k < 10; k++, j++) {
			Kbhit(s, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[13;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			tempp.wallet[k] = ch;
		}
		tempp.wallet[k] = '\0';
		//i = 0;
		while (!feof(inp)) {
			fread(&p, sizeof(patient), 1, inp);
			if (strcmp(p.name, "") == 0) {
				tempp.ID = i;
				break;
			}
			i++;
		}
		fseek(inp, tempp.ID * sizeof(patient), SEEK_SET);
		fwrite(&tempp, sizeof(patient), 1, inp);
		T.rent.numberp++;
		wprintf(L"\n\n \x1b[38;5;160mThe desired person has been added successfully.");
		Kbhit(s, 2);
		getch();
		//Sleep(3000);
		fclose(inp);
		break;
	case '2':
		listpatients();
		break;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter 1 or 2:\n\x1b[38;5;232m ");
		Kbhit(s, 2);
		goto line4;
	}
}
void listpatients() {
	FILE* inp = fopen("patientsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "rb+");
	int i, j, k;
	system("cls"); system("color F0"); wprintf(L"\033[?25l\n\n\n  ");
	for (i = 0; i < 83; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n |\x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient name               \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient ID   \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient national code   \x1b[38;5;%dm\x0004 \x1b[38;5;232mWallet     \x1b[38;5;240m|\n  ",teme,teme,teme,teme);
	for (i = 0; i < 83; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	while (fread(&p, sizeof(patient), 1, inp)) {
		if (strcmp(p.name, "") != 0) {
			wprintf(L"\n \x1b[38;5;240m|\x1b[38;5;232m  %S\033[34G%d\033[49G%S\033[75G%S\033[86G\x1b[38;5;240m|", p.name, p.ID, p.meli, p.wallet);

		}
	}
	wprintf(L"\n  ");
	for (i = 0; i < 83; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the ID to view the patient's turns.\n ",teme);
	scanf("%d", &l); // l is patient'ID
	getchar();
	fcloseall();
	allappointments(l);
	return;
}
void monthlyschedule() {
	char str[16];
	FILE* currentcal = fopen("current month.bin", "rb+");
	system("cls"); system("color F0");
	wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease choose an option:\n",teme);
	wprintf(L"\n 1- The calendar of current month\n 2- The calendar of next month\n ");
	wprintf(L"\033[2;2H");
	const char* s = "Monthly schedule";
	Kbhit(s, 2);
line5:	wprintf(L"\033[8;2H");
	ch = _getche();
	switch (ch) {
	case '1':
		rewind(currentcal);
		if (fread(str, sizeof str, 1, currentcal) == 0) {
			system("cls"); system("color F0");

		line11:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the start date of the complex:\n\n \x1b[38;5;%dm[     ]",teme,teme);
			wprintf(L"\033[5;4H\x1b[38;5;232m");
			int a; scanf("%d", &a);
			if (a < 1 || a > 31 || a < curentry->tm_mday) {
				wprintf(L"\n \a\x1b[38;5;160mPlease enter the correct input."); Sleep(2500);
				wprintf(L"\033[1;2H\033[0J");
				goto line11;
			}
		}
		setcalendar(0, 0, 0);
		break;
	case '2':
		setcalendar(1, 0, 0);
		break;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter 1 or 2:\n\x1b[38;5;232m ");
		Kbhit(s, 2);
		goto line5;
	}
}
void setcalendar(int whatmonth, int view, int closuredate) { // whatmonth is 0 for current month and 1 for next month and view is 1 if the function viewcalendar called setcalendar
	FILE* currentcal = fopen("current month.bin", "rb+"); // closuredate is the date we want to change the status of its closure
	FILE* nextcal = fopen("next month.bin", "rb+");
	FILE* fp = NULL;
	//int mod, numcur, numnext;//num is the number of days in current month
	//char res[51];
	for (int i = 0; i < 42; i++) {
		mon[i] = { 0,0,{0},0,0,{{0,0,{{0,0,0,0,{0},{0},{0},{0} }}}} };
	}
	switch (curentry->tm_mon) {
	case 0:
		numcur = 31;
		if ((curentry->tm_year + 1900) % 100 == 0 && (curentry->tm_year + 1900) % 400 == 0)
			numnext = 29;
		else if ((curentry->tm_year + 1900) % 4 == 0 && (curentry->tm_year + 1900) % 100 != 0)
			numnext = 29;
		else numnext = 28;
		break;
	case 1:
		if ((curentry->tm_year + 1900) % 100 == 0 && (curentry->tm_year + 1900) % 400 == 0)
			numcur = 29;
		else if ((curentry->tm_year + 1900) % 4 == 0 && (curentry->tm_year + 1900) % 100 != 0)
			numcur = 29;
		else numcur = 28;
		numnext = 31;
		break;
	case 2: numcur = 31; numnext = 30;  break;
	case 3: numcur = 30; numnext = 31; break;
	case 4: numcur = 31; numnext = 30;  break;
	case 5: numcur = 30; numnext = 31; break;
	case 6: numcur = 31; numnext = 31; break;
	case 7: numcur = 31; numnext = 30; break;
	case 8: numcur = 30; numnext = 31; break;
	case 9: numcur = 31; numnext = 30; break;
	case 10: numcur = 30; numnext = 31; break;
	case 11: numcur = 31; numnext = 31; break;
	}
	int first;     mod = (numcur - curentry->tm_mday) % 7 + curentry->tm_wday;     mod %= 7;      first = mod + 1;//این متغیر مشخص میکنه اولین روز ماه بعد چه روزی از هفته است 
	if (whatmonth == 0) {
		char str[16]; char year[5];  itoa(curentry->tm_year + 1900, year, 10);
		switch (curentry->tm_mon) {
		case 0: strcpy(str, "January"); break;
		case 1: strcpy(str, "February"); break;
		case 2: strcpy(str, "March"); break;
		case 3: strcpy(str, "April"); break;
		case 4: strcpy(str, "May"); break;
		case 5: strcpy(str, "June"); break;
		case 6: strcpy(str, "July"); break;
		case 7: strcpy(str, "August"); break;
		case 8: strcpy(str, "September"); break;
		case 9: strcpy(str, "October"); break;
		case 10: strcpy(str, "November"); break;
		case 11: strcpy(str, "December"); break;
		}
		strcat(str, " "); strcat(str, year);
		fp = currentcal;
		if (view == 1) {
			for (int i = 0; i < 42; i++) {
				if (mon[i]._date == closuredate) {
					if (mon[i].holiday == 1) {
						if (i % 7 == 0) {
							wprintf(L"\n \a\x1b[38;5;160mYou can't change the closing status of this day.");
						}
						else {
							mon[i].holiday = 0;
							strcpy(mon[i].reason, "");
							wprintf(L"\n \x1b[38;5;160mThe closure changed successfully.");
						}
						//Sleep(3000);
						Kbhit("Calendar of current month", 2);
						_getch();
						break;
					}
					else if (mon[i].holiday == 0) {
						int j, k, ch;
						mon[i].holiday = 1;
						wprintf(L"\n \x1b[38;5;232mWhat is the reason of closure?\n ");
						for (k = 0, j = 4; k < 50; k++, j++) {
							Kbhit("Calendar of current month", 2);
							wprintf(L"\033[?25h");
							wprintf(L"\033[20;%dH\x1b[38;5;232m", j);
							ch = _getche();
							if (ch == '\r') {
								break;
							}
							if (ch == '\b') {
								k -= 2;
								j -= 2;
								continue;
							}
							res[k] = ch;
						}
						res[k] = '\0';
						//gets_s(res);
						strcpy(mon[i].reason, res);
						wprintf(L"\n \x1b[38;5;160mThe closure changed successfully.");
						Kbhit("Calendar of current month", 2);
						_getch();
						break;
					}
				}
			}
			fwrite(str, sizeof str, 1, fp); // sizeof str = 16
			fwrite(mon, sizeof mon, 1, fp);
			fclose(currentcal);
			fclose(nextcal);
		}
		else if (!fread(str, sizeof str, 1, fp)) {
			int tarikh = curentry->tm_mday;//tarikh rooz
			int week = curentry->tm_wday;// tarikh system kodam rooz hafte ast
			mod = ((tarikh - 1) / 7 + 1) * 7 + 1 - tarikh + week;
			mod %= 7;
			for (int i = mod, t = 1; i < numcur + mod; t++, i++) {
				mon[i]._date = t;
				if (i % 7 == 0) {
					mon[i].holiday = 1;
					strcpy(mon[i].reason, "Sunday");
				}
				else if (i % 7 == 6) {
					mon[i].beginworktime = 8;
					mon[i].endworktime = 13;
				}
				else {
					mon[i].beginworktime = 8;
					mon[i].endworktime = 21;
				}
			}
			fwrite(str, sizeof str, 1, fp); // sizeof str = 16
			fwrite(mon, sizeof mon, 1, fp);
			fclose(currentcal);
			fclose(nextcal);
		}
		viewcalendar(whatmonth, numcur);
	}
	else if (whatmonth == 1) {
		fp = nextcal; char str[16]; char year[5];
		if (curentry->tm_mon != 11) {
			itoa(curentry->tm_year + 1900, year, 10);
			switch (curentry->tm_mon + 1) {
			case 0: strcpy(str, "January"); break;
			case 1: strcpy(str, "February"); break;
			case 2: strcpy(str, "March"); break;
			case 3: strcpy(str, "April"); break;
			case 4: strcpy(str, "May"); break;
			case 5: strcpy(str, "June"); break;
			case 6: strcpy(str, "July"); break;
			case 7: strcpy(str, "August"); break;
			case 8: strcpy(str, "September"); break;
			case 9: strcpy(str, "October"); break;
			case 10: strcpy(str, "November"); break;
			case 11: strcpy(str, "December"); break;
			}
			strcat(str, " "); strcat(str, year);
		}
		if (curentry->tm_mon == 11) {
			itoa(curentry->tm_year + 1901, year, 10);
			strcpy(str, "January");
			strcat(str, " "); strcat(str, year);
		}
		if (view == 1) {
			for (int i = 0; i < 42; i++) {
				if (mon[i]._date == closuredate) {
					if (mon[i].holiday == 1) {
						if (i % 7 == 0) {
							wprintf(L"\n \a\x1b[38;5;160mYou can't change the closing status of this day.");
						}
						else {
							mon[i].holiday = 0;
							strcpy(mon[i].reason, "");
							wprintf(L"\n \x1b[38;5;160mThe closure changed successfully.");
						}
						//Sleep(3000);
						Kbhit("Calendar of current month", 2);
						_getch();
						break;
					}
					else if (mon[i].holiday == 0) {
						int j, k, ch;
						mon[i].holiday = 1;
						wprintf(L"\n \x1b[38;5;232mWhat is the reason of closure?\n ");
						for (k = 0, j = 4; k < 50; k++, j++) {
							Kbhit("Calendar of current month", 2);
							wprintf(L"\033[?25h");
							wprintf(L"\033[20;%dH\x1b[38;5;232m", j);
							ch = _getche();
							if (ch == '\r') {
								break;
							}
							if (ch == '\b') {
								k -= 2;
								j -= 2;
								continue;
							}
							res[k] = ch;
						}
						res[k] = '\0';
						//gets_s(res);
						strcpy(mon[i].reason, res);
						wprintf(L"\n \x1b[38;5;160mThe closure changed successfully.");
						Kbhit("Calendar of current month", 2);
						_getch();
						break;
					}
				}
			}
			fwrite(str, sizeof str, 1, fp); // sizeof str = 16
			fwrite(mon, sizeof mon, 1, fp);
			fclose(currentcal);
			fclose(nextcal);
		}
		else if (!fread(str, sizeof(str), 1, fp)) {
			for (int i = first, t = 1; i < first + numnext; t++, i++) {
				mon[i]._date = t;
				if (i % 7 == 0) {
					mon[i].holiday = 1;
					strcpy(mon[i].reason, "Sunday");
				}
				else if (i % 7 == 6) {
					mon[i].beginworktime = 8;
					mon[i].endworktime = 13;
				}
				else {
					mon[i].beginworktime = 8;
					mon[i].endworktime = 21;
				}
			}
			fwrite(str, sizeof str, 1, fp); // sizeof str = 16
			fwrite(mon, sizeof mon, 1, fp);
			fclose(currentcal);
			fclose(nextcal);
		}
		viewcalendar(whatmonth, numnext);
	}
}
void viewcalendar(int whatmonth, int num) { // num is the number of days in month
	system("cls"); system("color F0"); wprintf(L"\033[?25l");
	FILE* currentcal = fopen("current month.bin", "rb+");
	FILE* nextcal = fopen("next month.bin", "rb+");
	FILE* fp = NULL;
	char str[16];
	wprintf(L"\n\n\n\n\n  ");
	for (int i = 0; i < 34; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n |\x1b[38;5;232mSun. Mon. Tue. Wed. Thu. Fri. Sat.\x1b[38;5;240m|\n  ");
	for (int i = 0; i < 34; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	if (whatmonth == 0) {
		fp = currentcal;
		fread(str, sizeof(str), 1, fp);
		fread(mon, sizeof mon, 1, fp);
		wprintf(L"\033[5;2H \x1b[38;5;%dm%S\n",teme, str);
		wprintf(L"\033[9;2H");
		for (int i = 0; i < 42; i++) {
			if (mon[i]._date == 0) {
				if (i % 7 != 6)
					wprintf(L"\x1b[38;5;240m|    ");
				else {
					wprintf(L"\x1b[38;5;240m|    |\n ");
				}
			}
			else if (mon[i]._date < curentry->tm_mday) {
				if (mon[i]._date / 10 == 0) {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;240m%d  ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;240m%d  \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
				else {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;240m%d ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;240m%d \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
			}
			else if (mon[i].holiday == 1 && mon[i]._date >= curentry->tm_mday) {
				if (mon[i]._date / 10 == 0) {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d  ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d  \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
				else {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
			}
			else {
				if (mon[i]._date / 10 == 0) {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d  ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d  \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
				else {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
			}
		}
	}
	else if (whatmonth == 1) {
		fp = nextcal;
		fread(str, sizeof(str), 1, fp);
		fread(mon, sizeof mon, 1, fp);
		wprintf(L"\033[5;2H\x1b[38;5;%dm%S\n ",teme, str);
		wprintf(L"\033[9;2H");

		for (int i = 0; i < 42; i++) {
			if (mon[i]._date == 0) {
				if (i % 7 != 6)
					wprintf(L"\x1b[38;5;240m|    ");
				else {
					wprintf(L"\x1b[38;5;240m|    |\n ");
				}
			}

			else if (mon[i].holiday == 1) {
				if (mon[i]._date / 10 == 0) {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d  ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d  \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
				else {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;160m%d \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
			}
			else {
				if (mon[i]._date / 10 == 0) {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d  ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d  \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
				else {
					if (i % 7 != 6)
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d ", mon[i]._date);
					else {
						wprintf(L"\x1b[38;5;240m| \x1b[38;5;232m%d \x1b[38;5;240m|\n ", mon[i]._date);
					}
				}
			}
		}
	}
	wprintf(L" ");
	for (int i = 0; i < 34; i++)
		wprintf(L"\x1b[38;5;240m\x0336");

	char f[3], nums[3], dat[3]; int k, j; char ch;
line6:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mTo see the reason for the closure or to change the closure status of a day, enter the date, otherwise, enter the number 0.\n ",teme);

	if (whatmonth == 0) {
		const char* st = "Calendar of current month"; wprintf(L"\033[?25l");   Kbhit(st, 2);
		for (k = 0, j = 4; k < 3; k++, j++) {
			Kbhit(st, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[18;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			f[k] = ch;
		}
		f[k] = '\0';

		if (strcmp(f, "0") == 0)
			return;
		else if (strcmp(f, "1") >= 0 && atoi(f) <= num) {
			for (int i = 0; i < 42; i++) {
				if (strcmp(f, itoa(mon[i]._date, dat, 10)) == 0) {
					if (mon[i].holiday == 0) {
						if (atoi(f) - curentry->tm_mday >= 2)
							setcalendar(whatmonth, 1, atoi(f));
						else {
							wprintf(L"\n \a\x1b[38;5;160mYou can't change the closing status of this day.");
							//Sleep(3000);
							Kbhit(st, 2);
							_getch();
							wprintf(L"\033[16;2H\033[0J\033[A");
							goto line6;
						}
					}
					else if (mon[i].holiday == 1) {
						char s[4];
						wprintf(L"\n \x1b[38;5;160m%S\n", mon[i].reason);
					line10:	wprintf(L"\n \x1b[38;5;232mDo you want to change the closing status of this day?(Yes/No)\n ");
						for (k = 0, j = 4; k < 3; k++, j++) {
							Kbhit(st, 2);
							wprintf(L"\033[?25h");
							wprintf(L"\033[22;%dH\x1b[38;5;232m", j);
							ch = _getche();
							if (ch == '\r') {
								break;
							}
							if (ch == '\b') {
								k -= 2;
								j -= 2;
								continue;
							}
							s[k] = ch;
						}
						s[k] = '\0';
						strlwr(s);
						if (strcmp(s, "yes") == 0) {
							if (atoi(f) - curentry->tm_mday >= 2)
								setcalendar(whatmonth, 1, atoi(f));
							else {
								wprintf(L"\n \a\x1b[38;5;160mYou can't change the closing status of this day.");
								//	Sleep(3000);
								Kbhit(st, 2);
								_getch();
								wprintf(L"\033[16;2H\033[0J\033[A");
								//getchar();
								goto line6;
							}
						}
						else if (strcmp(s, "no") == 0) {
							//	getchar();
							wprintf(L"\033[16;2H\033[0J\033[A");
							goto line6;
						}
						else {
							wprintf(L"\n\n\a \x1b[38;5;160mPlease enter the correct input.");
							Kbhit(st, 2);
							_getch();
							wprintf(L"\033[21;2H\033[0J\033[A");
							goto line10;
						}
					}
					break;
				}
			}
		}
		else {
			wprintf(L"\n\n\a \x1b[38;5;160mPlease enter the correct input.");
			Kbhit(st, 2);
			_getch();
			wprintf(L"\033[16;2H\033[0J\033[A");
			goto line6;
		}

	}
	else {
		const char* st = "Calendar of next month";    wprintf(L"\033[?25l");   Kbhit(st, 2);
		for (k = 0, j = 4; k < 3; k++, j++) {
			Kbhit(st, 2);
			wprintf(L"\033[?25h");
			wprintf(L"\033[18;%dH\x1b[38;5;232m", j);
			ch = _getche();
			if (ch == '\r') {
				break;
			}
			if (ch == '\b') {
				k -= 2;
				j -= 2;
				continue;
			}
			f[k] = ch;
		}
		f[k] = '\0';
		if (strcmp(f, "0") == 0)
			return;
		else if (strcmp(f, "1") >= 0 && atoi(f) <= num) {
			for (int i = 0; i < 42; i++) {
				if (strcmp(f, itoa(mon[i]._date, dat, 10)) == 0) {
					if (mon[i].holiday == 0) {
						setcalendar(whatmonth, 1, atoi(f));
					}
					else if (mon[i].holiday == 1) {
						char s[4];
						wprintf(L"\n \x1b[38;5;160m%S\n", mon[i].reason);
					line7:	wprintf(L"\n \x1b[38;5;232mDo you want to change the closing status of this day?(Yes/No)\n ");
						for (k = 0, j = 4; k < 3; k++, j++) {
							Kbhit(st, 2);
							wprintf(L"\033[?25h");
							wprintf(L"\033[22;%dH\x1b[38;5;232m", j);
							ch = _getche();
							if (ch == '\r') {
								break;
							}
							if (ch == '\b') {
								k -= 2;
								j -= 2;
								continue;
							}
							s[k] = ch;
						}
						s[k] = '\0';
						strlwr(s);
						if (strcmp(s, "yes") == 0) {
							setcalendar(whatmonth, 1, atoi(f));
						}
						else if (strcmp(s, "no") == 0) {
							//getchar();
							wprintf(L"\033[16;2H\033[0J\033[A");
							goto line6;
						}
						else {
							wprintf(L"\n\n\a \x1b[38;5;160mPlease enter the correct input.");
							Kbhit(st, 2);
							_getch();
							wprintf(L"\033[21;2H\033[0J\033[A");
							goto line7;
						}
					}
					break;
				}
			}
		}
		else {
			wprintf(L"\n\n\a \x1b[38;5;160mPlease enter the correct input.");
			Kbhit(st, 2);
			_getch();
			wprintf(L"\033[16;2H\033[0J\033[A");
			goto line6;
		}
	}
	fcloseall();
}
void visitsschedule() {
	system("cls");    system("color F0"); wprintf(L"\033[?25l");
	const char* st = "Visits schedule";
	wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mplease choose an option:\n\n 1- All\n 2- Choose Doctor\n ",teme);
	Kbhit(st, 2);
line22:	wprintf(L"\033[8;2H");
	ch = getche();
	switch (ch) {
	case '1':
		all();
		break;
	case '2':
		choosedoctor();
		break;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter a number from 1 to 5:\n\x1b[38;5;232m ");
		Kbhit(st, 2);
		goto line22;
	}
}
void choosedoctor() {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "rb+");
	char stt[16];
	fread(stt, sizeof stt, 1, currentcal);
	for (int i = 0; i < 42; i++)
		fread(&mon[i], sizeof(date), 1, currentcal);
	system("cls");   system("color F0");    wprintf(L"\033[?25l\n\n\n  ");    count = 6;
	for (int i = 0; i < 73; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n | \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor ID     \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor Name                    \x1b[38;5;%dm\x0004 \x1b[38;5;232mExpertise            \x1b[38;5;240m|\n  ",teme,teme,teme);
	for (int i = 0; i < 73; i++)
		wprintf(L"\x1b[38;5;240m\x0336");

	while (fread(&d, sizeof(doctor), 1, ind)) {
		if (strcmp(d.name, "") != 0) {
			wprintf(L"\n \x1b[38;5;240m| \x1b[38;5;232m  %d\033[22G%S\033[55G%S \033[76G\x1b[38;5;240m|", d.ID, d.name, d.expertise);
			count++;
		}
	}
	wprintf(L"\n  ");
	for (int i = 0; i < 73; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the ID of the desired doctor.\n ",teme);
	count += 4;
	int a;
	scanf("%d", &a);

	int i, k, j, numdays;//numdays is the number of days in current month
	for (i = 0; i < 42; i++) {
		if (mon[i]._date == 1)
			break;
	}// we need the amount of i
	numdays = Numdays();
	system("cls"); system("color F0"); wprintf(L"\033[?25l\n\n  ");   line = 6;
	for (k = 0; k < 91; k++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n |\x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment date     \x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment hour     \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient name                   \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient ID\x1b[38;5;240m|\n  ",teme,teme,teme,teme);
	for (k = 0; k < 91; k++)
		wprintf(L"\x1b[38;5;240m\x0336"); wprintf(L"\n ");
	for (j = i; j < i + numdays; j++) {
		strcat(itoa(mon[j]._date, q, 10), "th");
		//printf the mon[i]._date 
		for (k = 0; k < 156; k++) {
			if (mon[j].doctorsshift[a].arrturns[k].reservedorno == 1) {
				wprintf(L"\x1b[38;5;240m|  \x1b[38;5;232m%S\033[28G%S\033[51G%S\033[84G%d\033[94G\x1b[38;5;240m|\n ", q, mon[j].doctorsshift[a].arrturns[k].hour, mon[j].doctorsshift[a].arrturns[k].patienrname, mon[j].doctorsshift[a].arrturns[k].patientID);
				// print the information of turn
				count++;
				line++;
			}
		}
	}
	wprintf(L" ");
	for (k = 0; k < 91; k++)
		wprintf(L"\x1b[38;5;240m\x0336");
line30:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter 0 to exit.\n  ",teme);
	getchar();
	scanf("%c", &ch);
	getchar();
	if (ch == '0') {
		fcloseall();
		return;
	}
	else {
		wprintf(L"\n \a\x1b[38;5;160mPlease enter correct input.");
		Sleep(2500);
		wprintf(L"\033[%d;2H\033[0J\033[2A", line + 2);
		goto line30;
	}
}
void all() {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "rb+");
	char stt[16];
	fread(stt, sizeof stt, 1, currentcal);
	for (int i = 0; i < 42; i++)
		fread(&mon[i], sizeof(date), 1, currentcal);
	strcpy(q, "");
	int i, k, j, numdays;//numdays is the number of days in current month
	for (i = 0; i < 42; i++) {
		if (mon[i]._date == 1)
			break;
	}// we need the amount of i
	numdays = Numdays();
	system("cls"); system("color F0"); wprintf(L"\033[?25l\n\n\n  ");   count = 6;
	for (int g = 0; g < 125; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n |\x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment date   \x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment hour   \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor name               \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor ID   \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient name               \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient ID\x1b[38;5;240m|\n  ",teme,teme,teme,teme,teme,teme);
	for (int g = 0; g < 125; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	for (j = i; j < i + numdays; j++) {
		strcat(itoa(mon[j]._date, q, 10), "th");
		for (k = 0; k < 50; k++) {
			for (int h = 0; h < 156; h++) {
				if (mon[j].doctorsshift[k].arrturns[h].reservedorno == 1) {
					wprintf(L"\n \x1b[38;5;240m|  \x1b[38;5;232m%S\033[26G%S\033[47G%S\033[75G%d\033[89G%S\033[118G%d\033[128G\x1b[38;5;240m|", q, mon[j].doctorsshift[k].arrturns[h].hour, mon[j].doctorsshift[k].arrturns[h].doctorname, mon[j].doctorsshift[k].arrturns[h].doctorID, mon[j].doctorsshift[k].arrturns[h].patienrname, mon[j].doctorsshift[k].arrturns[h].patientID);
					count++;
				}
			}
		}
	}
	wprintf(L"\n  ");
	for (int g = 0; g < 125; g++)
		wprintf(L"\x1b[38;5;240m\x0336");

line31:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter 0 to exit.\n  ",teme);
	//getchar();
	scanf("%c", &ch);
	getchar();
	if (ch == '0') {
		fcloseall();
		return;
	}
	else {
		wprintf(L"\n \a\x1b[38;5;160mPlease enter correct input.");
		Sleep(2500);
		wprintf(L"\033[%d;2H\033[0J\033[A", count + 2);
		goto line31;
	}
}

void doctoraccount(int ID) {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	system("cls");
	system("color F0");
	wprintf(L"\033[?25l");
	wprintf(L"\n \x1b[38;5;232mWelcome to ");
	wprintf(L"\n\n \x1b[38;5;232m1- Determining the shifts\n 2- Reserved Visits\n 3- Patient Prescription\n 4- Rent Payment\n 5- Visirs Payment\n 6- Wallet\n 7- Exit\n ");
	const char* s = "your account";    const char* wallet = "Wallet";
	Kbhit(s, 13);
line12:	wprintf(L"\033[11;2H");
	ch = getche();
	switch (ch) {
	case '1':
		Determiningshifts(ID);
		doctoraccount(ID);
		break;
	case '2':
		reservedvisits(ID);
		doctoraccount(ID);
		break;
	case '3':
		patientprescription(ID);
		doctoraccount(ID);
		break;
	case '4':
		rentpayment(ID);
		doctoraccount(ID);
		break;
	case '5':
		rewind(ind);
		system("cls");
		system("color F0");
		while (fread(&d, sizeof(doctor), 1, ind)) {
			if (d.ID == ID)
				break;
		}
	line14:	wprintf(L"\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the amount and time span of your visit:\n\n Visirs payment:\n \x1b[38;5;%dm[      ]\n\n \x1b[38;5;232mTime span:(in minutes)\n \x1b[38;5;%dm[      ]",teme,teme,teme);
		wprintf(L"\033[3A");  wprintf(L"\033[6D\x1b[38;5;232m");
		scanf("%d", &d.visirpayment);
		if (d.visirpayment < 10 || d.visirpayment>50) {
			wprintf(L"\n\n\n\n \a\x1b[38;5;160mThe cost range is not appropriate.");
			Sleep(2500);
			wprintf(L"\033[1;1H\033[0J");
			goto line14;
		}
		wprintf(L"\033[2B\033[3C");
		scanf("%d", &d.visittime);
		rewind(ind);
		while (fread(&temp, sizeof(doctor), 1, ind)) {
			if (temp.ID == ID) {
				fseek(ind, ID * sizeof(doctor), SEEK_SET);
				int a = fwrite(&d, sizeof(doctor), 1, ind);
				fseek(ind, ID * sizeof(doctor), SEEK_SET);
				fread(&temp, sizeof(doctor), 1, ind);
				break;
			}
		}
		wprintf(L"\n \x1b[38;5;160mThe price and time span of the visit was successfully registered.");
		Sleep(3000);
		doctoraccount(ID);
		break;
	case '6':
		system("cls");      system("color F0");   wprintf(L"\033[?25l");
		rewind(ind);
		while (fread(&d, sizeof(doctor), 1, ind)) {
			if (d.ID == ID)
				break;
		}
		wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mThe amount available in your wallet: %d $",teme, d.wallet);
		Kbhit(wallet, 2);
		getch();
		doctoraccount(ID);
		break;
	case '7':
		//	getchar();
		mainpage();
		break;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter a number from 1 to 6:\n\x1b[38;5;232m ");
		Kbhit(s, 13);
		goto line12;
	}
	fclose(ind);
}
void Determiningshifts(int ID) {
	FILE* currentcal = fopen("current month.bin", "rb+");
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	if (currentcal == NULL)
		return;

	char hourshift[12], str[16], temp[12], hourtemp[12];
	int token = 0, dat, min = 0, max = 0;
	fread(str, sizeof str, 1, currentcal);
	int c = fread(mon, sizeof mon, 1, currentcal);
	system("cls");   system("color F0");    wprintf(L"\033[?25l");
	const char* s = "Determining the shifts";
	while (fread(&d, sizeof(doctor), 1, ind)) {
		if (d.ID == ID) {
			if (d.visittime == 0 || d.visirpayment == 0) {
				wprintf(L"\n\n\n\a \x1b[38;5;160mPlease complete section 5 (Visirs payment and time span) first and then enter this section.");
				Kbhit(s, 2);
				getch();
				fcloseall();
				return;
			}
			break;
		}
	}
	wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease choose an option:\n\n 1-Set for a day\n 2-Recurring days in the month\n ",teme);
	Kbhit(s, 2);
line15:	wprintf(L"\033[8;2H");
	ch = getche();
	switch (ch) {
	case '1':
	line16:	system("cls"); system("color F0");
		wprintf(L"\n\n \x1b[38;5;232mPlease enter the date of day:\n \x1b[38;5;%dm[    ]\n\n \x1b[38;5;232mPlease enter the hour of shift:(for example:15:30-20:15)\n \x1b[38;5;%dm[             ]",teme,teme);
		wprintf(L"\033[3A"); wprintf(L"\033[13D\x1b[38;5;232m");
		scanf("%d", &dat); wprintf(L"\033[2B\033[3C");
		scanf("%s", hourshift);
		strcpy(temp, hourshift);    min = atoi(strtok(temp, ":")); strtok(NULL, "-");    max = atoi(strtok(NULL, ":"));
		for (int i = 0; i < 42; i++) {
			if (mon[i]._date == dat) {
				if (min<mon[i].beginworktime || max>mon[i].endworktime) {
					wprintf(L"\n \a\x1b[38;5;160mThe complex is closed in this time.");
					Sleep(2500);
					goto line16;
				}
				if (mon[i].holiday == 1) {
					wprintf(L"\n \a\x1b[38;5;160mThe complex is closed on this day.");
					Sleep(2500);
					goto line16;
				}
				if (mon[i]._date < curentry->tm_mday) {
					wprintf(L"\n \a\x1b[38;5;160mThis day has passed.");
					Sleep(2500);
					goto line16;
				}
				int numturns = numberturns(hourshift, d.visittime);
				strcpy(tok, strtok(hourshift, "-"));
				for (int j = 0; j < numturns; j++) {
					strcpy(tem, tok);
					strcpy(mon[i].doctorsshift[ID].arrturns[j].hour, hourvisit(tem, d.visittime, j));
					mon[i].doctorsshift[ID].arrturns[j].doctorID = ID;
					strcpy(mon[i].doctorsshift[ID].arrturns[j].doctorname, d.name);
					strcpy(mon[i].doctorsshift[ID].arrturns[j].expertise, d.expertise);
					mon[i].doctorsshift[ID].arrturns[j].turnpayment = d.visirpayment;
					mon[i].doctorsshift[ID].arrturns[j].reservedorno = 0;
				}
				rewind(currentcal);
				fwrite(str, sizeof str, 1, currentcal);
				for (int i = 0; i < 42; i++)
					fwrite(&mon[i], sizeof(date), 1, currentcal);
				wprintf(L"\n \x1b[38;5;160mThe shift was registered successfully.");
				Sleep(2500);
				break;
			}
		}
		break;
	case '2':
	line23:system("cls"); system("color F0");
		wprintf(L"\n\n \x1b[38;5;232mPlease enter the day of the week that you will shift repeatedly:\n \x1b[38;5;%dm[           ]\n\n \x1b[38;5;232mPlease enter the hour of shift:(for example:15:30-20:15)\n \x1b[38;5;%dm[             ]",teme,teme);
		wprintf(L"\033[3A\033[13D\x1b[38;5;232m");
		gets_s(week);  strlwr(week);
		wprintf(L"\033[2B\033[3C");
		gets_s(hourshift);
		strcpy(temp, hourshift);    min = atoi(strtok(temp, ":")); strtok(NULL, "-");    max = atoi(strtok(NULL, ":"));
		int k;
		if (strcmp(week, "sunday") == 0) {
			wprintf(L"\n \a\x1b[38;5;160mThe complex is closed on Sundays.");
			Sleep(2500);
			goto line23;
		}
		//	i = sunday;
		else if (strcmp(week, "monday") == 0)
			k = monday;
		else if (strcmp(week, "tuesday") == 0)
			k = tuesday;
		else if (strcmp(week, "wednesday") == 0)
			k = wednesday;
		else if (strcmp(week, "thursday") == 0)
			k = thursday;
		else if (strcmp(week, "friday") == 0)
			k = friday;
		else if (strcmp(week, "saturday") == 0)
			k = saturday;
		if (min<mon[k].beginworktime || max>mon[k].endworktime) {
			wprintf(L"\n \a\x1b[38;5;160mThe complex is closed in this time.");
			Sleep(2500);
			goto line23;
		}
		strcpy(srr, "The complex is closed in "); char a[3];
		for (int i = k; i < 42; i += 7) {
			strcpy(hourtemp, hourshift);
			if (mon[i].holiday == 1 || mon[i]._date < curentry->tm_mday) {
				if (mon[i]._date != 0) {
					strcat(srr, itoa(mon[i]._date, a, 10));     strcat(srr, "th,");
				}
			}
			else {
				int numturns = numberturns(hourshift, d.visittime);
				strcpy(tok, strtok(hourtemp, "-"));
				for (int j = 0; j < numturns; j++) {
					strcpy(tem, tok);
					strcpy(mon[i].doctorsshift[ID].arrturns[j].hour, hourvisit(tem, d.visittime, j));
					mon[i].doctorsshift[ID].arrturns[j].doctorID = ID;
					strcpy(mon[i].doctorsshift[ID].arrturns[j].doctorname, d.name);
					strcpy(mon[i].doctorsshift[ID].arrturns[j].expertise, d.expertise);
					mon[i].doctorsshift[ID].arrturns[j].turnpayment = d.visirpayment;
					mon[i].doctorsshift[ID].arrturns[j].reservedorno = 0;
				}
			}
		}
		l = strlen(srr);  srr[l - 1] = '\0';
		rewind(currentcal);
		fwrite(str, sizeof str, 1, currentcal);
		fwrite(mon, sizeof mon, 1, currentcal);
		wprintf(L"\n \x1b[38;5;160m%S but the shifts on other days were registered successfully.", srr);
		Sleep(5000);
		break;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter 1 or 2:\n\x1b[38;5;232m ");
		Kbhit(s, 2);
		goto line15;
	}
	fcloseall();
}
int numberturns(char* hourshift, int visittime) {
	char temp[12], sec1[6], sec2[6];
	strcpy(temp, hourshift);   strcpy(sec1, strtok(temp, "-"));   strcpy(sec2, strtok(NULL, "-"));
	int h1 = atoi(strtok(sec1, ":"));    int m1 = atoi(strtok(NULL, ":"));
	int h2 = atoi(strtok(sec2, ":"));    int m2 = atoi(strtok(NULL, ":"));
	m1 = h1 * 60 + m1;         m2 = h2 * 60 + m2;
	return (m2 - m1) / visittime;
}
char* hourvisit(char* start, int visittime, int loopindex) {
	char* res = (char*)malloc(sizeof(char) * 6);
	strcpy(res, "");
	int h = atoi(strtok(start, ":"));     int m = atoi(strtok(NULL, ":"));
	m = 60 * h + m;
	m += visittime * loopindex;
	char a[3];
	itoa(m / 60, a, 10); strcat(res, a); strcat(res, ":");   itoa(m % 60, a, 10);  char d[3];  strcpy(d, "0");
	if (m % 60 < 10)
		strcat(d, a);
	else strcpy(d, a);
	strcat(res, d);
	return res;
}
void reservedvisits(int ID) {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "rb+");
	system("cls"); system("color F0"); wprintf(L"\033[?25l");
	wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mTo view reserved appointments in a time period, select that period:forexample(24-30)\n \x1b[38;5;%dm[       ]",teme,teme);
	const char* s = "Reserved Visits";  int j, k, i;
	Kbhit(s, 2);
	for (k = 0, j = 4; k < 33; k++, j++) {
		Kbhit(s, 2);
		wprintf(L"\033[?25h");
		wprintf(L"\033[5;%dH\x1b[38;5;232m", j);
		ch = _getche();
		if (ch == '\r') {
			break;
		}
		if (ch == '\b') {
			k -= 2;
			j -= 2;
			continue;
		}
		q[k] = ch;
	}
	q[k] = '\0';
	int min = atoi(strtok(q, "-")), max = atoi(strtok(NULL, "-"));    count = 0;
	l = max - min + 1;//the number of days that we must print their turns
	char str[16];
	fread(str, sizeof str, 1, currentcal);
	fread(mon, sizeof mon, 1, currentcal);
	for (i = 0; i < 42; i++) {
		if (mon[i]._date == min)
			break;
	}// we need the amount of i
	system("cls"); system("color F0"); wprintf(L"\033[?25l\n\n  ");   line = 6;
	for (k = 0; k < 91; k++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n |\x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment date     \x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment hour     \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient name                   \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient ID\x1b[38;5;240m|\n  ",teme,teme,teme,teme);
	for (k = 0; k < 91; k++)
		wprintf(L"\x1b[38;5;240m\x0336"); wprintf(L"\n ");
	for (j = i; j < i + l; j++) {
		strcat(itoa(mon[j]._date, q, 10), "th");
		//printf the mon[i]._date 
		for (k = 0; k < 156; k++) {
			if (mon[j].doctorsshift[ID].arrturns[k].reservedorno == 1) {
				wprintf(L"\x1b[38;5;240m|  \x1b[38;5;232m%S\033[28G%S\033[51G%S\033[84G%d\033[94G\x1b[38;5;240m|\n ", q, mon[j].doctorsshift[ID].arrturns[k].hour, mon[j].doctorsshift[ID].arrturns[k].patienrname, mon[j].doctorsshift[ID].arrturns[k].patientID);
				// print the information of turn
				count++;
				line++;
			}
		}
	}
	wprintf(L" ");
	for (k = 0; k < 91; k++)
		wprintf(L"\x1b[38;5;240m\x0336");
	if (count == 0) {
		wprintf(L"\n\n \x1b[38;5;160mNo appointment was found for you in the specified time period.");
		line += 2;
		//Sleep(5000);
	}
line29:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter 0 to exit.\n  ",teme);
	scanf("%c", &ch);
	getchar();
	if (ch == '0') {
		fcloseall();
		return;
	}
	else {
		wprintf(L"\n \a\x1b[38;5;160mPlease enter correct input.");
		Sleep(2500);
		wprintf(L"\033[%d;2H\033[0J\033[2A", line + 2);
		goto line29;
	}
}
void patientprescription(int ID) {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* inp = fopen("patientsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "rb+");
	int i, j, k, numdays;
	numdays = Numdays();
	char stt[16];
	fread(stt, sizeof stt, 1, currentcal);
	for (int i = 0; i < 42; i++)
		fread(&mon[i], sizeof(date), 1, currentcal);
	for (i = 0; i < 42; i++) {
		if (mon[i]._date == 1)
			break;
	}// we need the amount of i
	strcpy(q, "");
	system("cls"); system("color F0"); wprintf(L"\033[?25l\n\n\n  ");  count = 6;
	for (int g = 0; g < 83; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n \x1b[38;5;240m|\x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment date   \x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment hour   \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient name               \x1b[38;5;%dm\x0004 \x1b[38;5;232mPatient ID\x1b[38;5;240m|\n  ",teme,teme,teme,teme);
	for (int g = 0; g < 83; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	for (j = i; j < i + numdays; j++) {
		strcat(itoa(mon[j]._date, q, 10), "th");
		for (k = 0; k < 156; k++) {
			if (mon[j].doctorsshift[ID].arrturns[k].reservedorno == 1) {
				wprintf(L"\n \x1b[38;5;240m|\x1b[38;5;232m  %S\033[26G%S\033[47G%S\033[76G%d\033[86G\x1b[38;5;240m|", q, mon[j].doctorsshift[ID].arrturns[k].hour, mon[j].doctorsshift[ID].arrturns[k].patienrname, mon[j].doctorsshift[ID].arrturns[k].patientID);
				count++;
				// print the enformation of patient
			}
		}
	}
	wprintf(L"\n  ");
	for (int g = 0; g < 83; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the ID of the desired patient.\n ",teme);
	count += 4;
	int a;
	scanf("%d", &a); //a is the patient ID
	fseek(inp, a * sizeof(patient), SEEK_SET);
	fread(&p, sizeof(patient), 1, inp);
	system("cls"); system("color F0"); wprintf(L"\033[?25l\n\n ");
	wprintf(L"\x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter (Ctrl + Z + Enter) after writing the patient prescription.\n\n  ",teme);
	wprintf(L"\x1b[38;5;%dmPrescription for the patient with ID %d:\n  ", teme,a);
	for (int g = 0; g < 57; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n");
	for (int g = 0; g < 15; g++)
		wprintf(L" |                                                         |\n");    wprintf(L"  ");
	for (int g = 0; g < 57; g++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\033[7;4H\x1b[38;5;232m");
	for (i = 0; (p.noskhearr[p.numnoskhe].noskhe[i] = getc(stdin)) != EOF; i++) {
		if (p.noskhearr[p.numnoskhe].noskhe[i] == '\n')
			wprintf(L"\033[4G");
	}
	p.noskhearr[p.numnoskhe].noskhe[i] = '\0';

	time_t r; tm* tt;  r = time(NULL);  tt = localtime(&r);   strcpy(temppp, "");
	strcat(temppp, asctime(tt));
	strcat(temppp, "Doctor name: ");   strcat(temppp, d.name);    strcat(temppp, "\n\n");     strcat(temppp, p.noskhearr[p.numnoskhe].noskhe);
	strcpy(p.noskhearr[p.numnoskhe].noskhe, temppp);
	wprintf(L"\033[15B \x1b[38;5;160mThe prescription was successfully registered and the visit fee was added to your wallet.");
	Sleep(7000);
	fseek(ind, ID * sizeof(doctor), SEEK_SET);
	fread(&d, sizeof(doctor), 1, ind);
	d.wallet += d.visirpayment;
	fseek(ind, ID * sizeof(doctor), SEEK_SET);
	fwrite(&d, sizeof(doctor), 1, ind);
	p.numnoskhe++;
	fseek(inp, a * sizeof(patient), SEEK_SET);
	fwrite(&p, sizeof(patient), 1, inp);
	fcloseall();
	return;
}
void rentpayment(int ID) {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	if (ind == NULL) return;

	while (!feof(ind)) {
		fread(&d, sizeof(doctor), 1, ind);
		if (d.ID == ID)
			break;
	}

	int numdays;// the number of days in month
	numdays = Numdays();
	system("cls");  system("color F0");
	const char* s = "Rent Payment";  	const char* st = "Payment";
	wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mPlease choose an optin:\n\n 1- Extension\n 2- Payment\n ",teme);
	Kbhit(s, 2);
line19:wprintf(L"\033[8;2H");
	ch = getche();
	switch (ch) {
	case '1':
		if (d.extension == 0) {
			d.extension = 1;
			T.rent.notpaymentthismonth[ID] = 1;
			T.rent.notpaymentthismonth[ID] += 2;
			fseek(ind, ID * sizeof(doctor), SEEK_SET);
			fwrite(&d, sizeof(doctor), 1, ind);
			wprintf(L"\n \x1b[38;5;160mThe request was successfully sent.");
			Kbhit(s, 2);
			getch();
			fcloseall();
			return;
		}
		else {
			temp = { "","","","","",0,10,0,0,0 };
			fseek(ind, ID * sizeof(doctor), SEEK_SET);
			fwrite(&temp, sizeof(doctor), 1, ind);
			T.rent.numberd--;
			wprintf(L"\n \x1b[38;5;160mYour account was deleted due to not paying office rent on time.");
			Kbhit(s, 2);
			getch();
			fcloseall();
			return;
		}
	case '2':
		system("cls"); system("color F0");
		if (d.extension == 0) {
			pay = numdays * 10;
			if (d.wallet >= pay) {
				d.wallet -= pay;
				T.rent.notpaymentthismonth[ID] = 1;
				wprintf(L"\n\n\n \x1b[38;5;160mPayment was successful.");
				Kbhit(st, 2);
				getch();
				fseek(ind, ID * sizeof(doctor), SEEK_SET);
				fwrite(&d, sizeof(doctor), 1, ind);
				fcloseall();
				return;
			}
			else {
				T.rent.notpaymentmonths[ID]++;
				T.rent.notpaymentthismonth[ID] = 1;
				wprintf(L"\n\n\n \a\x1b[38;5;160mYour wallet balance is not enough.");
				Kbhit(st, 2);
				getch();
				fcloseall();
				return;
			}
		}
		else if (d.extension == 1) {
			pay = numdays * 20;
			if (d.wallet >= pay) {
				T.rent.notpaymentthismonth[ID] = 1;
				T.rent.notpaymentmonths[ID] -= 2;
				d.wallet -= pay;
				d.extension = 0;
				wprintf(L"\n\n\n \x1b[38;5;160mPayment was successful.");
				Kbhit(st, 2);
				getch();
				fseek(ind, ID * sizeof(doctor), SEEK_SET);
				fwrite(&d, sizeof(doctor), 1, ind);
				fcloseall();
				return;
			}
			else {
				temp = { "","","","","",0,10,0,0,0 };
				fseek(ind, ID * sizeof(doctor), SEEK_SET);
				fwrite(&temp, sizeof(doctor), 1, ind);
				T.rent.numberd--;
				wprintf(L"\n \x1b[38;5;160mYour account was deleted due to not paying office rent on time.");
				fcloseall();
				return;
			}
		}
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter 1 or 2:\n\x1b[38;5;232m ");
		Kbhit(s, 2);
		goto line19;
	}
}

void patientaccount(int ID) {
	FILE* inp = fopen("patientsfile.bin", "rb+");
	system("cls");
	system("color F0");
	wprintf(L"\033[?25l");
	wprintf(L"\n \x1b[38;5;232mWelcome to ");
	wprintf(L"\n\n \x1b[38;5;232m1- Book an Appointmen\n 2- Cancel an Appointment\n 3- All Appointments\n 4- Prescriptions\n 5- Wallet\n 6- Exit\n ");
	const char* s = "your account";  const char* wallet = "Wallet";
	Kbhit(s, 13);
line25:	wprintf(L"\033[10;2H");
	ch = getche();
	switch (ch) {
	case '1':
		bookappointment(ID);
		patientaccount(ID);
		break;
	case '2':
		cancelappointment(ID);
		patientaccount(ID);
		break;
	case '3':
		allappointments(ID);
		patientaccount(ID);
		break;
	case '4':
		prescriptions(ID);
		patientaccount(ID);
		break;
	case '5':
		system("cls");      system("color F0");   wprintf(L"\033[?25l");
		rewind(inp);
		while (fread(&p, sizeof(patient), 1, inp)) {
			if (p.ID == ID)
				break;
		}
		wprintf(L"\n\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mThe amount available in your wallet: %S $",teme, p.wallet);
		Kbhit(wallet, 2);
		getch();
		patientaccount(ID);
		break;
	case '6':
		//getchar();
		mainpage();
		break;
	default:
		wprintf(L"\n \a\x1b[38;5;160mPlease enter a number from 1 to 5:\n\x1b[38;5;232m ");
		Kbhit(s, 13);
		goto line25;
	}
	//fclose(inp);  
}
void bookappointment(int ID) {
	FILE* ind = fopen("doctorsfile.bin", "rb+");
	FILE* inp = fopen("patientsfile.bin", "rb+");
	FILE* currentcal = fopen("current month.bin", "rb+");
	char stt[16];
	while (fread(&p, sizeof(patient), 1, inp)) {
		if (p.ID == ID)
			break;
	}
	fread(stt, sizeof stt, 1, currentcal);
	for (int i = 0; i < 42; i++)
		fread(&mon[i], sizeof(date), 1, currentcal);

	system("cls");   system("color F0");    wprintf(L"\033[?25l\n\n\n  ");    count = 6;
	for (int i = 0; i < 73; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n | \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor ID     \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor Name                    \x1b[38;5;%dm\x0004 \x1b[38;5;232mExpertise            \x1b[38;5;240m|\n  ",teme,teme,teme);
	for (int i = 0; i < 73; i++)
		wprintf(L"\x1b[38;5;240m\x0336");

	while (fread(&d, sizeof(doctor), 1, ind)) {
		if (strcmp(d.name, "") != 0) {
			wprintf(L"\n \x1b[38;5;240m| \x1b[38;5;232m  %d\033[22G%S\033[55G%S \033[76G\x1b[38;5;240m|", d.ID, d.name, d.expertise);
			count++;
		}
	}
	wprintf(L"\n  ");
	for (int i = 0; i < 73; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the ID of the desired doctor.\n ",teme);
	count += 4;
	int a;
	scanf("%d", &a);
	system("cls");   system("color F0");     wprintf(L"\033[?25l\n\n"); char str[14] = "", b[3];    count = 4;
	switch (curentry->tm_mon) {
	case 0: strcpy(str, "January"); break;
	case 1: strcpy(str, "February"); break;
	case 2: strcpy(str, "March"); break;
	case 3: strcpy(str, "April"); break;
	case 4: strcpy(str, "May"); break;
	case 5: strcpy(str, "June"); break;
	case 6: strcpy(str, "July"); break;
	case 7: strcpy(str, "August"); break;
	case 8: strcpy(str, "September"); break;
	case 9: strcpy(str, "October"); break;
	case 10: strcpy(str, "November"); break;
	case 11: strcpy(str, "December"); break;
	}
	l = strlen(str);
	int i, j, k;
	for (i = 0; i < 42; i++) {
		if (mon[i]._date > curentry->tm_mday) {
			strcat(str, " ");        strcat(str, itoa(mon[i]._date, b, 10));
			wprintf(L"\n  \x1b[38;5;%dm %S\n ",teme, str);   count += 2;
			for (int i = 0; i < 73; i++)
				wprintf(L"\x1b[38;5;240m\x0336");
			wprintf(L"\n ");   count++;
			for (j = 1; j <= 156; j++) {
				if (mon[i].doctorsshift[a].arrturns[j - 1].reservedorno == 0 && strcmp(mon[i].doctorsshift[a].arrturns[j - 1].hour, "") != 0) {
					if (j % 5 == 1) {
						wprintf(L"\n ");
						count++;
					}
					wprintf(L"\x1b[38;5;232m %d- %S\t", j, mon[i].doctorsshift[a].arrturns[j - 1].hour);
				}
				else if (mon[i].doctorsshift[a].arrturns[j - 1].reservedorno == 1) {
					if (j % 5 == 1) {
						wprintf(L"\n ");
						count++;
					}
					wprintf(L"\x1b[38;5;240m %d- %S\t", j, mon[i].doctorsshift[a].arrturns[j - 1].hour);
				}
			}
			str[l] = '\0';
			strcpy(b, "");
		}
	}
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the desired appointment code in the form of date + space + appointment number or 0 to exit.\n ",teme);
	int h;
	getchar();
	gets_s(q);
	if (strcmp(q, "0") == 0)
		return;
	int dat = atoi(strtok(q, " "));   int numturn = atoi(strtok(NULL, " "));
	system("cls"); system("color F0");
	rewind(ind);
	while (fread(&d, sizeof(doctor), 1, ind)) {
		if (d.ID == a)
			break;
	}
	int f = atoi(p.wallet);
	if (f < 1.1 * d.visirpayment) {
		wprintf(L"\n \a\x1b[38;5;160mYour wallet balance is not enough.");
		Sleep(3000);
	}
	else {
		rewind(currentcal);
		for (int i = 0; i < 42; i++) {
			if (mon[i]._date == dat) {
				if (mon[i].doctorsshift[a].arrturns[numturn - 1].reservedorno == 1) {
					wprintf(L"\n \a\x1b[38;5;160mThis appointment has already been booked.");
					Sleep(3500);
					fcloseall();
					return;
				}
				mon[i].doctorsshift[a].arrturns[numturn - 1].patientID = ID;
				strcpy(mon[i].doctorsshift[a].arrturns[numturn - 1].patienrname, p.name);
				mon[i].doctorsshift[a].arrturns[numturn - 1].reservedorno = 1;
				f -= (int)1.1 * d.visirpayment;
				p.turnsinfo[p.numturns].doctor_ID = a;
				p.turnsinfo[p.numturns].turn_date = dat;
				p.numturns++;
				break;
			}
		}
		fclose(currentcal);
		FILE* currentcal = fopen("current month.bin", "wb");
		fwrite(stt, sizeof stt, 1, currentcal);
		fwrite(mon, sizeof mon, 1, currentcal);
		fseek(inp, ID * sizeof(patient), SEEK_SET);
		fwrite(&p, sizeof(patient), 1, inp);
		wprintf(L"\n\x1b[38;5;160m Payment was successful.");
		Sleep(2000);
	}
	fcloseall();
}
void cancelappointment(int ID) {
	FILE* currentcal = fopen("current month.bin", "rb+");
	FILE* inp = fopen("patientsfile.bin", "rb+");
	char stt[16];   fread(stt, sizeof stt, 1, currentcal);      fread(mon, sizeof mon, 1, currentcal);
	fseek(inp, ID * sizeof(patient), SEEK_SET);      fread(&p, sizeof(patient), 1, inp);

	int i, j, k;   strcpy(q, "");
	system("cls");  system("color F0");  wprintf(L"\033[?25l\n\n\n  ");   count = 6;
	for (i = 0; i < 110; i++)
		wprintf(L"\x1b[38;5;240m\x0336");  wprintf(L"\n");
	wprintf(L" |\x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment date   \x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment hour   \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor name               \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor ID   \x1b[38;5;%dm\x0004 \x1b[38;5;232mExpertise               \x1b[38;5;240m|\n  ",teme,teme,teme,teme,teme);
	for (i = 0; i < 110; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	for (i = 0; i < 50; i++) {
		strcat(itoa(p.turnsinfo[i].turn_date, q, 10), "th");
		if (p.turnsinfo[i].turn_date != 0) {
			for (j = 0; j < 42; j++) {
				if (mon[j]._date == p.turnsinfo[i].turn_date)
					break;
			}//we need j
			for (k = 0; k < 156; k++) {
				if (mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].patientID == ID && strcmp("", mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].hour) != 0, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].reservedorno == 1) {
					wprintf(L"\n \x1b[38;5;240m|\x1b[38;5;232m  %S\033[26G%S\033[47G%S\033[75G%d\033[89G%S\033[113G\x1b[38;5;240m|", q, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].hour, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].doctorname, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].doctorID, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].expertise);
					count++;
				}
			}
			//			break;
		}
	}
	wprintf(L"\n  ");
	for (i = 0; i < 110; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter the appointment code you want to cancel in the form of date + space + doctor ID.\n ",teme);
	gets_s(q);  int dat = atoi(strtok(q, " "));    int doctorid = atoi(strtok(NULL, " "));
	for (i = 0; i < 42; i++) {
		if (mon[i]._date == dat)
			break;
	}// we need i
	for (j = 0; j < 156; j++) {
		if (mon[i].doctorsshift[doctorid].arrturns[j].patientID == ID && mon[i].doctorsshift[doctorid].arrturns[j].reservedorno == 1) {
			mon[i].doctorsshift[doctorid].arrturns[j].reservedorno = 0;
			mon[i].doctorsshift[doctorid].arrturns[j].patientID = 0;
			strcpy(mon[i].doctorsshift[doctorid].arrturns[j].patienrname, "");
			strcpy(p.wallet, itoa((int)((0.5 * mon[i].doctorsshift[doctorid].arrturns[j].turnpayment) + atoi(p.wallet)), www, 10));
			p.numturns--;
			for (k = 0; k < 50; k++) {
				if (p.turnsinfo[k].turn_date == dat && p.turnsinfo[k].doctor_ID == doctorid) {
					for (int g = k; g < 50; g++) {
						if (g != 49)
							p.turnsinfo[g] = p.turnsinfo[g + 1];
						else p.turnsinfo[g] = { 0,0 };

					}
				}
			}
			break;
		}
	}
	rewind(currentcal);
	fwrite(stt, sizeof stt, 1, currentcal);
	fwrite(mon, sizeof mon, 1, currentcal);
	fseek(inp, ID * sizeof(patient), SEEK_SET);
	fwrite(&p, sizeof(patient), 1, inp);
	wprintf(L"\n \x1b[38;5;160mThe appointment was successfully canceled.");
	Sleep(3000);
	fcloseall();
	return;
}
void allappointments(int ID) {
	FILE* currentcal = fopen("current month.bin", "rb+");
	FILE* inp = fopen("patientsfile.bin", "rb+");
	char stt[16];   fread(stt, sizeof stt, 1, currentcal);      fread(mon, sizeof mon, 1, currentcal);
	fseek(inp, ID * sizeof(patient), SEEK_SET);      fread(&p, sizeof(patient), 1, inp);

	int i, j, k;   strcpy(q, "");
	system("cls");  system("color F0");  wprintf(L"\033[?25l\n\n\n  ");   count = 6;
	for (i = 0; i < 110; i++)
		wprintf(L"\x1b[38;5;240m\x0336");  wprintf(L"\n");
	wprintf(L" |\x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment date   \x1b[38;5;%dm\x0004 \x1b[38;5;232mAppointment hour   \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor name               \x1b[38;5;%dm\x0004 \x1b[38;5;232mDoctor ID   \x1b[38;5;%dm\x0004 \x1b[38;5;232mExpertise               \x1b[38;5;240m|\n  ",teme,teme,teme,teme,teme);
	for (i = 0; i < 110; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
	for (i = 0; i < 50; i++) {
		strcat(itoa(p.turnsinfo[i].turn_date, q, 10), "th");
		if (p.turnsinfo[i].turn_date != 0) {
			for (j = 0; j < 42; j++) {
				if (mon[j]._date == p.turnsinfo[i].turn_date)
					break;
			}//we need j
			for (k = 0; k < 156; k++) {
				if (mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].patientID == ID && strcmp("", mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].hour) != 0, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].reservedorno == 1) {
					wprintf(L"\n \x1b[38;5;240m|\x1b[38;5;232m  %S\033[26G%S\033[47G%S\033[75G%d\033[89G%S\033[113G\x1b[38;5;240m|", q, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].hour, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].doctorname, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].doctorID, mon[j].doctorsshift[p.turnsinfo[i].doctor_ID].arrturns[k].expertise);
					count++;
				}
			}
			//			break;
		}
	}
	wprintf(L"\n  ");
	for (i = 0; i < 110; i++)
		wprintf(L"\x1b[38;5;240m\x0336");
line35:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter 0 to exit.\n  ",teme);
	scanf("%c", &ch);
	getchar();
	if (ch == '0') {
		fcloseall();
		return;
	}
	else {
		wprintf(L"\n \a\x1b[38;5;160mPlease enter correct input.");
		Sleep(2500);
		wprintf(L"\033[%d;2H\033[0J\033[A", count + 2);
		goto line35;
	}
}
void prescriptions(int ID) {
	FILE* inp = fopen("patientsfile.bin", "rb+");

	fseek(inp, ID * sizeof(patient), SEEK_SET);
	fread(&p, sizeof(patient), 1, inp);
	system("cls");   system("color F0");    wprintf(L"\033[?25l\n\n\n  ");    line = 4;
	for (int i = 0; i < 30; i++) {
		count = 0;
		if (p.noskhearr[i].noskhe[0] != '\0') {
			for (int g = 0; g < 57; g++)
				wprintf(L"\x1b[38;5;240m\x0336");
			wprintf(L"\n");   line += 3;
			for (int g = 0; g < 15; g++)
				wprintf(L" |                                                         |\n");    wprintf(L"  ");
			for (int g = 0; g < 57; g++)
				wprintf(L"\x1b[38;5;240m\x0336");
			wprintf(L"\033[%d;4H", 17 * i + 6);
			for (int j = 0; p.noskhearr[i].noskhe[j] != '\0'; j++) {
				if (p.noskhearr[i].noskhe[j] == '\n') {
					wprintf(L"%c\033[4G\x1b[38;5;232m", p.noskhearr[i].noskhe[j]);
					count++;
					line++;
				}
				else {
					if (count == 0 || count == 1 || count == 2)
						wprintf(L"\x1b[38;5;%dm%c", p.noskhearr[i].noskhe[j]);
					else wprintf(L"\x1b[38;5;232m%c", p.noskhearr[i].noskhe[j]);
				}
			}
			wprintf(L"\033[%dB\033[D", 17 * i + 8);
			line += 17 * i + 8;
		}
	}
line33:	wprintf(L"\n\n \x1b[38;5;%dm\x0004 \x1b[38;5;232mEnter 0 to exit.\n  ",teme);
	scanf("%c", &ch);
	getchar();
	if (ch == '0') {
		fcloseall();
		return;
	}
	else {
		wprintf(L"\n \a\x1b[38;5;160mPlease enter correct input.");
		Sleep(2500);
		wprintf(L"\033[%d;2H\033[0J\033[5A", line + 2);
		goto line33;
	}
}