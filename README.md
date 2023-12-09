# Assignment 2 : LTE in NS3 (WCMC)
------
## Group Member : NeeraJ Nikhil Roy(20/11/EC/053) & Kshama Meena (20/11/EC/055)
------
 **Steps to run this code**

 - Step 1. Install and set-up NS3 from official website
_Install the ns3.40 fro the officila website_
_You could refer to its official documentation and Guides on how to install it._
_Here is the link for the website: https://www.nsnam.org/releases/ns-3-40/_

> Do note that you have to enable examples, debuging, test and disable the errors as warning during the build.
> Use CMAKE in vscode or from comand line or any other prefered method to install the ns3

 - Step 2. Clone this repository to your home or any prefered directory.
 - Step 3. Replace the `pf-ff-mac-scheduler.cc` and `pf-ff-mac-scheduler.h` files with the existing files in your `ns-allinone-<version>/ns-<version>/src/lte/model` directory.
 - Step 4. Change directory to `ns-allinone-<version>/ns-<version>/scratch` and copy `Rollno53_55.cc` from repository to here.
 - Step 5. Go back and run `./ns3 run scratch/Rollno53_55.cc`
Change the comand to run according to your needs. Here is a sample run command `./ns3 run scratch/Rollno53_55.cc --command="%s --speed={double} --scheduler={scheduler class name} --full_buffer={0, 1} --RngRun={seed value}"`
> According to your requirments change the code such as to get rem plot uncoment those lines and run or change schedulers or log info etc.

---
**How to plot the graph**

After performing the first step of running the code, you will get `rem.out` and other outout files such as `sinr_instantaneous_throughput.txt`, `throughput_cdf.txt` and `sinr_data.txt`.
This will contain the SINR and other data for the topology.
 - Step 1. Open terminal and go to the directory where `rem.out` exists
 - Step 2. Copy the ploting files from the `gnuplotscripts` folder alonng with slightly modifying the output file names accordingly. This is just done to easily distinguish the data files. 
 - Step 3. Run the following command in sequence to plot using gnuplot
 
        gnuplot plot_file_name.plt

You will get the graph files acordingly
Here in the repository you could find the data collleted are kept in `data collected` folder just copy the data fo there to a directory along with corresponding respecctive plot files fromm the `gnupotscripts` folder and plot them using the commanf given above.
---
>**Credits and Socials**
> Neeraj Nikhil Roy  🐶🐶🐶
> Mail: neerajroy06502@gmail.com, neeraj51_soe@jnu.ac.in
> Instagram: https://www.instagram.com/blacklistperformer/
> Github: https://github.com/blacklistperformer
> Linkedln: https://www.linkedin.com/in/neeraj-roy-556968192/ 
> Stacoverflow: https://stackoverflow.com/users/19916561/neeraj-roy
>
> Kshama Meena ❤️❤️❤️
> Mail: kshamameena7@gmail.com, kshama65_soe@jnu.ac.in
> Instagram: https://www.instagram.com/meenakshama_11_/ 
> Github: https://github.com/kshamameena
> Linkedln: https://www.linkedin.com/in/kshama-meena-1851a8207/ 
---
