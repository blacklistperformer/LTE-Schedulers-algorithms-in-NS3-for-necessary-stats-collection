//------------------------------------------------
// Including all the neccsary / important liabraries and headers.
#include "ns3/config-store.h"
#include "ns3/core-module.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/radio-bearer-stats-calculator.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/pf-ff-mac-scheduler.h"
#include <iomanip>
#include <string>
#include <map>
//------------------------------------------------

// Defining the naespace for the code
using namespace ns3;
//------------------------------------------------

// Maps to store various statistics
std::map<uint64_t, double> time_of_creation; // Map to store packet creation times
std::map<std::string, double> node_based_ttt; // Map to store time-to-transmit for each node
std::map<std::string, int> packet_counter; // Map to store the number of packets for each node
std::map<uint64_t, int> time_sinr; // Map to store SINR values over time


//------------------------------------------------
// DECLARING REQUIRED FUNCTIONS

// Function to print the packet transmission
void packet_transmitted(std::string context, const Ptr<const Packet> packet, const Address &srcAddress, const Address &destAddress)
{
    Time time = Simulator::Now();
    time_of_creation.insert(time_of_creation.end(), std::pair<uint64_t, double>(packet->GetUid(), time.GetSeconds()));
    Ipv4Address src = InetSocketAddress::ConvertFrom(srcAddress).GetIpv4();
    Ipv4Address dest = InetSocketAddress::ConvertFrom(destAddress).GetIpv4();
    // NS_LOG_UNCOND("Node: " << context << " Time: " << Simulator::Now().GetSeconds() << "s Packet Size: " << packet->GetSize() << " SOURCE: " << src << " DEST: " << dest);

    // This function is called when a packet is transmitted.
    // It records the creation time of the packet.
}

// Function to handle packet reception
void packet_recieved(std::string context, const Ptr<const Packet> packet, const Address &address)
{
    double tt = Simulator::Now().GetSeconds();
    auto it = time_of_creation.find(packet->GetUid());
    tt -= it->second;
    // NS_LOG_UNCOND("Packet recieved at time: " << Simulator::Now().GetSeconds());
    time_of_creation.erase(packet->GetUid());
    node_based_ttt[context] += tt;
    packet_counter[context] += 1;
    NS_LOG_UNCOND("Node: " << context << " Time: " << Simulator::Now().GetSeconds() << "s Packet Size: " << packet->GetSize() << " Time to transmit: " << tt << "s");

    // This function is called when a packet is received.
    // It calculates the time taken for the packet to be transmitted and updates statistics.

}

// Function to track SINR (Signal-to-Interference-plus-Noise Ratio)
void sinr_tracking(std::string context, uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId)
{
    ns3::ue_rnti = rnti; //RNTI stands for Radio Network Temporary Identifier. It's used to identify a UE (User Equipment) when an RRC (Radio Resource Control) connection exist
    uint64_t current_time = Simulator::Now().GetMilliSeconds();
    int log_sinr = 10*std::log10(sinr);

    // Open the file in write mode
    std::ofstream outputFile("sinr_data.txt", std::ios::app);


    time_sinr.insert(time_sinr.end(), std::pair<uint64_t, int>(current_time, log_sinr));
    NS_LOG_UNCOND("Time: " << current_time << "ms SINR: " << log_sinr << "dB");

    // Check if the file is open
    if (outputFile.is_open())
    {
        // Write data to the file
        outputFile << current_time << " " << log_sinr << std::endl;

        // Close the file
        outputFile.close();
    }
    else
    {
        NS_LOG_UNCOND("Error: Unable to open the file for writing.");
    }

    NS_LOG_UNCOND("Time: " << current_time << "ms SINR: " << log_sinr << "dB");



    // This function is called to track SINR values.
    // It calculates SINR in dB and records it over time.
}

// Function to print SINR and instantaneous throughput values
void print_sinr_instant(Ptr<OutputStreamWrapper> stream)
{
    if (time_sinr.size() > 0 && instantaneous_throughput.size()> 0)
    {
        for(auto it = instantaneous_throughput.begin(); it != instantaneous_throughput.end(); ++it)
        {
            NS_LOG_UNCOND("simulation time: " << (uint64_t)((*it).first) << "ms Sinr: " << time_sinr[(*it).first] << " instantaneous throughput: " << (*it).second);
            *stream->GetStream() << (uint64_t)((*it).first) << "\t" << time_sinr[(*it).first] << "\t" << (*it).second << std::endl;
        }
    }
    else
    {
        NS_LOG_UNCOND("time_sinr size: " << time_sinr.size() << " instantaneous throughput: " << instantaneous_throughput.size());
    }

       // This function prints SINR and instantaneous throughput values.
}

//graph 3
// Function to calculate average system throughput
double avg_system_throughput()
{
    std::vector<double> throughput;
    for (auto it = node_based_ttt.begin(); it != node_based_ttt.end(); ++it)
    {
        int total_bytes = packet_counter.find(it->first)->second * 1500;
        double avg_throughput = (double)total_bytes / it->second;
        throughput.push_back(avg_throughput);
    }

    // // Open a text file for writing
    // std::ofstream outputFile("throughput_values.txt");
    // if (!outputFile.is_open()) {
    //     std::cerr << "Error: Unable to open the file for writing." << std::endl;
    //     return -1;  // Indicate an error
    // }
    // // Write throughput values to the file
    // for (auto it = throughput.begin(); it != throughput.end(); ++it) {
    //     outputFile << *it << "\n";
    // }

    // outputFile.close();  // Close the file

    double total_system_throughput = 0.0;
    for(auto it = throughput.begin(); it != throughput.end(); ++it)
    {
        total_system_throughput += *it;
    }
    return total_system_throughput / (double)throughput.size();
    // This function calculates the average system throughput.
}

// Function to generate throughput Cumulative Distribution Function (CDF)
void throughput_cdf(Ptr<OutputStreamWrapper> stream)
{
    std::vector<double> throughput;
    for (auto it = node_based_ttt.begin(); it != node_based_ttt.end(); ++it) {
        int total_bytes = packet_counter.find(it->first)->second * 1500;
        double avg_throughput = (double)total_bytes / it->second;
        throughput.push_back(avg_throughput);
    }
    int index = 0;
    std::sort(throughput.begin(), throughput.end());
    for (auto it = throughput.begin(); it != throughput.end(); ++it) {
        *stream->GetStream() << ++index << "\t" << *it << std::endl;
    }
    // This function generates the throughput Cumulative Distribution Function (CDF).
}

//------------------------------------------------
// MAIN FUNCION
int main(int argc, char* argv[])
{
    //------------------------------------------------
    Config::SetDefault("ns3::LteEnbPhy::TxPower", DoubleValue(40.0));
    // LogComponentEnable("PfFfMacScheduler", LOG_INFO);

    // Setting up the paraeters as mentioned in the problem statement
    // Simulation parameters and configurations
    double enbDist = 5000.0;
    double radius = 500.0;
    uint32_t numUes = 10;
    double simTime = 30.0;
    int full_buffer_flag = 0;
    char scheduler[] = "ns3::PfFfMacScheduler";
    UintegerValue runValue;
    uint32_t numEnB = 4;
    double speed = 0.0;

    // Initialize NS-3 components
    CommandLine cmd(__FILE__);
    cmd.AddValue("scheduler", "the scheduler to use [ns3::PfFfMacScheduler]", scheduler);
    cmd.AddValue("full_buffer", "Full Buffer or Non Full Buffer [Non Full Buffer]", full_buffer_flag);
    cmd.AddValue("speed", "speed with which ue's are moving [10.0]", speed);
    cmd.AddValue("simTime", "simulation run time [30.0]", simTime);
    cmd.Parse(argc, argv);

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();
    cmd.Parse(argc, argv);

    // checking the Random Generator
    GlobalValue::GetValueByName("RngRun", runValue);

    // setting the speed
    Ptr<ns3::ConstantRandomVariable> x = CreateObject<ns3::ConstantRandomVariable>();
    x->SetAttribute("Constant", ns3::DoubleValue(speed));

    // setting the on time
    Ptr<ns3::ConstantRandomVariable> Ontime = CreateObject<ns3::ConstantRandomVariable>();
    Ptr<ns3::ConstantRandomVariable> Offtime = CreateObject<ns3::ConstantRandomVariable>();
    if (full_buffer_flag)
    {
        Ontime->SetAttribute("Constant", ns3::DoubleValue(0.000125));
        Offtime->SetAttribute("Constant", ns3::DoubleValue(0.0001));
    }
    else
    {
        Ontime->SetAttribute("Constant", ns3::DoubleValue(0.00125));
        Offtime->SetAttribute("Constant", ns3::DoubleValue(0.001));
    }

    // Create Nodes: eNodeB and UE
    NodeContainer enbNodes;
    NodeContainer ueNodes1;
    NodeContainer ueNodes2;
    NodeContainer ueNodes3;
    NodeContainer ueNodes4;
    enbNodes.Create(numEnB);
    ueNodes1.Create(numUes);
    ueNodes2.Create(numUes);
    ueNodes3.Create(numUes);
    ueNodes4.Create(numUes);

    // lte Helper and EPC helper
    Ptr<ns3::LteHelper> lteHelper = CreateObject<ns3::LteHelper>();
    Ptr<ns3::PointToPointEpcHelper> epcHelper = CreateObject<ns3::PointToPointEpcHelper>();
    lteHelper->SetEpcHelper(epcHelper);
    lteHelper->SetSchedulerType(scheduler);
    lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(50));
    lteHelper->SetEnbDeviceAttribute("DlBandwidth", UintegerValue(50));
    std::cout << "RngRun:" << runValue.Get() << std::endl;

    Ptr<Node> pgw = epcHelper->GetPgwNode();

    // Create a single RemoteHost
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    // Create the Internet
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("1Gbps")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    // interface 0 is localhost, 1 is p2p device

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    // Position of eNBs
    Ptr<ListPositionAllocator> PositionAlloc = CreateObject<ListPositionAllocator>();
    PositionAlloc->Add(Vector(0.0, 0.0, 0.0));
    PositionAlloc->Add(Vector(enbDist, 0.0, 0.0));
    PositionAlloc->Add(Vector(0.0, enbDist, 0.0));
    PositionAlloc->Add(Vector(enbDist, enbDist, 0.0));

    // Additional mobility model setup and configuration can be added here
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(PositionAlloc);
    enbMobility.Install(enbNodes);

    // Set up positions and mobility models for eNBs and UEs
    // Position of UEs for eNB 1
    MobilityHelper uemobility;
    uemobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator", "X", DoubleValue(0.0), "Y", DoubleValue(0.0), "rho", DoubleValue(radius));
    uemobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Mode", EnumValue(ns3::RandomWalk2dMobilityModel::Mode::MODE_TIME), "Bounds", RectangleValue(Rectangle(-1000.0, 6000.0, -1000.0, 6000.0)), "Time", TimeValue(Time("1s")), "Speed", PointerValue(x));
    uemobility.Install(ueNodes1);

    // // Position of UEs for eNB 2
    uemobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator", "X", DoubleValue(enbDist), "Y", DoubleValue(0.0), "rho", DoubleValue(radius));
    uemobility.Install(ueNodes2);

    // // Position of UEs for eNB 3
    uemobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator", "X", DoubleValue(0.0), "Y", DoubleValue(enbDist), "rho", DoubleValue(radius));
    uemobility.Install(ueNodes3);

    // Position of UEs for eNB 4
    uemobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator", "X", DoubleValue(enbDist), "Y", DoubleValue(enbDist), "rho", DoubleValue(radius));
    uemobility.Install(ueNodes4);

    // // Attach a trace to print postion whenever Course Changes
    // Config::Connect("/NodeList/*/$ns3::MobilityModel/$ns3::RandomWalk2dMobilityModel/CourseChange", MakeCallback(&print_position));

    // Create Devices and install them in the Nodes (eNB and UE)
    NetDeviceContainer enbDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueDevs1 = lteHelper->InstallUeDevice(ueNodes1);
    NetDeviceContainer ueDevs2 = lteHelper->InstallUeDevice(ueNodes2);
    NetDeviceContainer ueDevs3 = lteHelper->InstallUeDevice(ueNodes3);
    NetDeviceContainer ueDevs4 = lteHelper->InstallUeDevice(ueNodes4);

    // Install the IP stack on the UEs
    internet.Install(ueNodes1);
    internet.Install(ueNodes2);
    internet.Install(ueNodes3);
    internet.Install(ueNodes4);

    ns3::Ipv4InterfaceContainer ueIpIface1, ueIpIface2, ueIpIface3, ueIpIface4;

    ueIpIface1 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs1));
    ueIpIface2 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs2));
    ueIpIface3 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs3));
    ueIpIface4 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs4));

    // Assign IP address to UEs, and install applications
    for (uint32_t u = 0; u < ueNodes1.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes1.Get(u);
        // Set default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    for (uint32_t u = 0; u < ueNodes2.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes2.Get(u);
        // Set default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    for (uint32_t u = 0; u < ueNodes3.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes3.Get(u);
        // Set default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    for (uint32_t u = 0; u < ueNodes4.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes4.Get(u);
        // Set default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    // Attach UEs to a eNB
    lteHelper->Attach(ueDevs1, enbDevs.Get(0));
    lteHelper->Attach(ueDevs2, enbDevs.Get(1));
    lteHelper->Attach(ueDevs3, enbDevs.Get(2));
    lteHelper->Attach(ueDevs4, enbDevs.Get(3));

    // Install and start applications on UEs and remote host
    uint16_t dlPort = 1100;
    // uint16_t otherPort = 3000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;


    // Create OnOffClient application to send data from UEs to remoteHost
    for (uint32_t u=0; u < ueNodes1.GetN(); ++u)
    {
        // Installing Server application on UE
        PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
        serverApps.Add(dlPacketSinkHelper.Install(ueNodes1.Get(u)));

        // OnOffHelper clientHelper("ns3::UdpSocketFactory", InetSocketAddress(ueIpIface1.GetAddress(u)));
        // if (full_buffer_flag)
        // {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("12Mbps")));
        // }
        // else
        // {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("1.2Mbps")));
        // }
        // clientHelper.SetAttribute("OnTime", ns3::PointerValue(Ontime));
        // clientHelper.SetAttribute("OffTime", ns3::PointerValue(Offtime));
        // clientHelper.SetAttribute("PacketSize", UintegerValue(1500));
        // clientApps.Add(clientHelper.Install(remoteHost));

        UdpClientHelper dlClient(ueIpIface1.GetAddress(u), dlPort);
        if (full_buffer_flag)
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("1ms")));
        }
        else
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("10ms")));
        }
        dlClient.SetAttribute("MaxPackets", UintegerValue(0));
        dlClient.SetAttribute("PacketSize", UintegerValue(1472));
        clientApps.Add(dlClient.Install(remoteHost));
    }

    for (uint32_t u=0; u < ueNodes2.GetN(); ++u)
    {
        PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
        serverApps.Add(dlPacketSinkHelper.Install(ueNodes2.Get(u)));

        // OnOffHelper clientHelper("ns3::UdpSocketFactory", InetSocketAddress(ueIpIface2.GetAddress(u)));
        // if (full_buffer_flag)
        // {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("12Mbps")));
        // }
        // else
        // {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("1.2Mbps")));
        // }
        // clientHelper.SetAttribute("OnTime", ns3::PointerValue(Ontime));
        // clientHelper.SetAttribute("OffTime", ns3::PointerValue(Offtime));
        // clientHelper.SetAttribute("PacketSize", UintegerValue(1500));
        // clientApps.Add(clientHelper.Install(remoteHost));

        UdpClientHelper dlClient(ueIpIface2.GetAddress(u), dlPort);
        if (full_buffer_flag)
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("1ms")));
        }
        else
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("10ms")));
        }
        dlClient.SetAttribute("MaxPackets", UintegerValue(0));
        dlClient.SetAttribute("PacketSize", UintegerValue(1472));
        clientApps.Add(dlClient.Install(remoteHost));
    }

    for (uint32_t u=0; u < ueNodes3.GetN(); ++u)
    {
        PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
        serverApps.Add(dlPacketSinkHelper.Install(ueNodes3.Get(u)));

        UdpClientHelper dlClient(ueIpIface3.GetAddress(u), dlPort);
        if (full_buffer_flag)
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("1ms")));
        }
        else
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("10ms")));
        }
        dlClient.SetAttribute("MaxPackets", UintegerValue(0));
        dlClient.SetAttribute("PacketSize", UintegerValue(1472));
        clientApps.Add(dlClient.Install(remoteHost));

        // OnOffHelper clientHelper("ns3::UdpSocketFactory", InetSocketAddress(ueIpIface3.GetAddress(u)));
        // if (full_buffer_flag)
        // {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("12Mbps")));
        // }
        // else
        // {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("1.2Mbps")));
        // }
        // clientHelper.SetAttribute("OnTime", ns3::PointerValue(Ontime));
        // clientHelper.SetAttribute("OffTime", ns3::PointerValue(Offtime));
        // clientHelper.SetAttribute("PacketSize", UintegerValue(1500));
        // clientApps.Add(clientHelper.Install(remoteHost));

    }

    for (uint32_t u=0; u < ueNodes4.GetN(); ++u)
    {
        PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
        serverApps.Add(dlPacketSinkHelper.Install(ueNodes4.Get(u)));

        UdpClientHelper dlClient(ueIpIface4.GetAddress(u), dlPort);
        if (full_buffer_flag)
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("1ms")));
        }
        else
        {
            dlClient.SetAttribute("Interval", TimeValue(Time("10ms")));
        }
        dlClient.SetAttribute("MaxPackets", UintegerValue(0));
        dlClient.SetAttribute("PacketSize", UintegerValue(1472));
        clientApps.Add(dlClient.Install(remoteHost));

        // OnOffHelper clientHelper("ns3::UdpSocketFactory", InetSocketAddress(ueIpIface4.GetAddress(u)));
        // if (full_buffer_flag) {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("12Mbps")));
        // }
        // else {
        //     clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("1.2Mbps")));
        // }
        // clientHelper.SetAttribute("OnTime", ns3::PointerValue(Ontime));
        // clientHelper.SetAttribute("OffTime", ns3::PointerValue(Offtime));
        // clientHelper.SetAttribute("PacketSize", UintegerValue(1500));
        // clientApps.Add(clientHelper.Install(remoteHost));

    }

    // Connect packet tracer to all applications
    Config::Connect("/NodeList/*/ApplicationList/*/$ns3::UdpClient/TxWithAddresses", MakeCallback(&packet_transmitted));

    Config::Connect("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx", MakeCallback(&packet_recieved));
    serverApps.Start(MilliSeconds(500));
    clientApps.Start(MilliSeconds(500));
    lteHelper->EnableTraces();
    // p2ph.EnablePcapAll("asstwo");

    Simulator::Stop(Seconds(simTime));

    // // REM Plotter code
    // // USE it to plot and only when plotting REM map and not for simulation.
    // Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper>();
    // remHelper->SetAttribute("Channel", PointerValue(lteHelper->GetDownlinkSpectrumChannel()));
    // remHelper->SetAttribute("OutputFile", StringValue("rem.out"));
    // remHelper->SetAttribute("XMin", DoubleValue(-2000.0));
    // remHelper->SetAttribute("XMax", DoubleValue(7000.0));
    // remHelper->SetAttribute("XRes", UintegerValue(100));
    // remHelper->SetAttribute("YMin", DoubleValue(-2000.0));
    // remHelper->SetAttribute("YMax", DoubleValue(7000.0));
    // remHelper->SetAttribute("YRes", UintegerValue(75));
    // remHelper->SetAttribute("Z", DoubleValue(0.0));
    // remHelper->Install();


    Ptr<LteUeNetDevice> ue0dev = DynamicCast<LteUeNetDevice, NetDevice>(ueDevs1.Get(0));
    Ptr<LteUePhy> ue0phy = ue0dev->GetPhy();

    // Check if the UE's physical layer exists
    if (ue0phy != nullptr)
    {
        if(ue0phy->TraceConnect("ReportCurrentCellRsrpSinr", "Sinr per Instant", MakeCallback(&sinr_tracking)))
        {
            NS_LOG_UNCOND("It works");
        }
        else
        {
            NS_LOG_UNCOND("It does not");
        }
    }
    // Run the simulation
    Simulator::Run();
    Simulator::Destroy();

    // Calculate and print average system throughput
    NS_LOG_UNCOND("Total Average Aggregrate System Throuput: " << avg_system_throughput() << std::endl);

    AsciiTraceHelper asciiTraceHelper;

    // Create output streams and print SINR and throughput information
    Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream("sinr_instantaneous_throughput.txt");
    print_sinr_instant(stream);

    // // Open a File stream to store the sinr of all UEs
    // Ptr<OutputStreamWrapper> sinr_stream = asciiTraceHelper.CreateFileStream("sinr.txt");
    // sinr_tracking();

    // Open a File stream to store the Throughput of all UEs
    Ptr<OutputStreamWrapper> throughput_stream = asciiTraceHelper.CreateFileStream("throughput_cdf.txt");
    throughput_cdf(throughput_stream);


    return 0;
}
