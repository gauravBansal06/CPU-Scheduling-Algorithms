            //First In First Out Scheduling
//Input File name :- "input.dat" (see reference input file)
//Output File name :- "fifo_out.dat"
//Compile:- g++ fifo.cpp -o fifo
//Run:- ./fifo
//Assuming processes in file are in sorted arrival time manner
#include<bits/stdc++.h>
using namespace std;

int main()
{
    string line;
    int n,last_comp, tsched, turn_around_time, response_time, waiting_time,tat_sum=0,rt_sum=0;

    ifstream infile("input.dat");                 
    ofstream outfile("fifo_out.dat");

    if(!infile)
    {
      cerr << "Datafile does not exist" << endl;
      exit(-1);
    }

    getline(infile,line);
    stringstream str(line);
    str >> n;                         //no. of processes
    
    vector<int> arrival(n),burst(n);

    for(int i=0; i < n; i++)
    {
       getline(infile,line);
       stringstream str(line);
       str >> arrival[i];
       str >> burst[i];
    }

    infile.close();            //read complete
    
   //FIFO scheduling calculations
    
    outfile << "PID\tTurnaround\tResponse\tWaiting" << endl;
    cout << "PID\tTurnaround\tResponse\tWaiting" << endl;

    for(int i=0; i < n; i++)
    {
       if(i==0)
         last_comp = 0;
       else
         last_comp = burst[i-1] + tsched;

       if(arrival[i] > last_comp)
         tsched = arrival[i];
       else
         tsched = last_comp;

       turn_around_time = tsched + burst[i] - arrival[i];            //calculations
       tat_sum += turn_around_time;
       response_time = tsched - arrival[i];
       rt_sum += response_time;
       waiting_time = response_time;

       outfile << i+1 << "\t\t" << turn_around_time << "\t\t  " << response_time << "\t\t\t" << waiting_time << endl;
       cout << i+1 << "\t" << turn_around_time << "\t\t" << response_time << "\t\t" << waiting_time << endl;
    }
    
    outfile << endl;
    cout << endl;
    outfile << "Average Turnaround Time = " << (float)tat_sum/n << endl;
    outfile << "Average Response Time = " << (float)rt_sum/n << endl;
    outfile << "Average Waiting Time = " << (float)rt_sum/n << endl;
    cout << "Average Turnaround Time = " << (float)tat_sum/n << endl;
    cout << "Average Response Time = " << (float)rt_sum/n << endl;
    cout << "Average Waiting Time = " << (float)rt_sum/n << endl;
    
    outfile.close();
   
    return 0;
}
