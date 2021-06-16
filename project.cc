  
 #include <iostream>
 #include <fstream>
 #include <string>
 #include <cassert>
 
 #include "ns3/core-module.h"
 #include "ns3/network-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/flow-monitor-helper.h"
 #include "ns3/ipv4-global-routing-helper.h"
 #include "ns3/netanim-module.h"
 

 
 /*
 
	A___E___      _____ S 
                \   /
        	  G  
	B___F___/ |   \_____R----Internet
	    | 	  |
	    C     D
   - A, B, C, D send messages to Server (S)
   - S replies to A/B/C/D (70% of the times)or forward the message to R (30% of the times)
 
   // A B C D E F G Server Router
   // 0 1 2 3 4 5 6 7      8

 */
 using namespace ns3;
 
 NS_LOG_COMPONENT_DEFINE ("SimpleGlobalRoutingExample");
 
 static void received_msg (Ptr<Socket> socket1, Ptr<Socket> socket2, Ptr<const Packet> p, const Address &srcAddress , const Address &dstAddress)
{
	std::cout << "::::: A packet received at the Server! Time:   " << Simulator::Now ().GetSeconds () << std::endl;
	
	Ptr<UniformRandomVariable> rand=CreateObject<UniformRandomVariable>();
	
	if(rand->GetValue(0.0,1.0)<=0.3){
		std::cout << "::::: Transmitting from Server to Router   "  << std::endl;
		socket1->Send (Create<Packet> (p->GetSize ()));
	}
	else{
		std::cout << "::::: Transmitting from Server(GW) to Controller   "  << std::endl;
		socket2->SendTo(Create<Packet> (p->GetSize ()),0,srcAddress);
	}
}


static void GenerateTraffic (Ptr<Socket> socket, Ptr<ExponentialRandomVariable> randomSize,	Ptr<ExponentialRandomVariable> randomTime)
{
	uint32_t pktSize = randomSize->GetInteger (); //Get random value for packet size
	std::cout << "::::: A packet is generate at Node "<< socket->GetNode ()->GetId () << " with size " << pktSize <<" bytes ! Time:   " << Simulator::Now ().GetSeconds () << std::endl;
	
	// We make sure that the message is at least 12 bytes. The minimum length of the UDP header. We would get error otherwise.
	if(pktSize<12){
		pktSize=12;
	}
	
	socket->Send (Create<Packet> (pktSize));

	Time pktInterval = Seconds(randomTime->GetValue ()); //Get random value for next packet generation time 
	Simulator::Schedule (pktInterval, &GenerateTraffic, socket, randomSize, randomTime); //Schedule next packet generation
}
 
 int 
 main (int argc, char *argv[])
 {
 
   // A B C D E F G Server Router
   // 0 1 2 3 4 5 6 7      8

   // Users may find it convenient to turn on explicit debugging
   // for selected modules; the below lines suggest how to do this
 #if 0 
   LogComponentEnable ("SimpleGlobalRoutingExample", LOG_LEVEL_INFO);
 #endif
 
   // Set up some default values for the simulation.  Use the 
   Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (100));
   Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("50kb/s"));
 
  
   bool enableFlowMonitor = true;
  
 

   NS_LOG_INFO ("Create nodes.");
   NodeContainer c;
   Address serverAddress;
   
   c.Create (9);
   NodeContainer nAnE = NodeContainer (c.Get (0), c.Get (4));
   NodeContainer nBnF = NodeContainer (c.Get (1), c.Get (5));
   NodeContainer nCnF = NodeContainer (c.Get (2), c.Get (5));
   NodeContainer nDnG = NodeContainer (c.Get (3), c.Get (6));
   NodeContainer nEnG = NodeContainer (c.Get (4), c.Get (6));
   NodeContainer nFnG = NodeContainer (c.Get (5), c.Get (6));
   
   
   NodeContainer nGnS = NodeContainer (c.Get (6), c.Get (7));
   NodeContainer nGnR = NodeContainer (c.Get (6), c.Get (8));
  
   
 
   InternetStackHelper internet;
   internet.Install (c); //install protocols stack for
 
   // We create the channels first without any IP addressing information
   NS_LOG_INFO ("Create channels.");
   PointToPointHelper p2p_1;
   PointToPointHelper p2p_2;
   PointToPointHelper p2p_3;
   p2p_1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
   p2p_1.SetChannelAttribute ("Delay", StringValue ("2ms"));
   p2p_2.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
   p2p_2.SetChannelAttribute ("Delay", StringValue ("2ms"));
   p2p_3.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
   p2p_3.SetChannelAttribute ("Delay", StringValue ("2ms"));
   
   NetDeviceContainer dAdE = p2p_1.Install (nAnE);
   NetDeviceContainer dBdF = p2p_1.Install (nBnF);
   NetDeviceContainer dCdF = p2p_1.Install (nCnF);
   NetDeviceContainer dDdG = p2p_1.Install (nDnG);
   NetDeviceContainer dEdG = p2p_1.Install (nEnG);
   
   NetDeviceContainer dFdG = p2p_2.Install (nFnG);
   NetDeviceContainer dGdR = p2p_2.Install (nGnR);
   
   NetDeviceContainer dGdS = p2p_3.Install (nGnS);
  
   // Later, we add IP addresses.
   NS_LOG_INFO ("Assign IP Addresses.");
   Ipv4AddressHelper ipv4;
   ipv4.SetBase ("10.1.1.0", "255.255.255.0");
   Ipv4InterfaceContainer iAiE = ipv4.Assign (dAdE);
 
   ipv4.SetBase ("10.1.2.0", "255.255.255.0");
   Ipv4InterfaceContainer iBiF = ipv4.Assign (dBdF);
 
   ipv4.SetBase ("10.1.3.0", "255.255.255.0");
   Ipv4InterfaceContainer iCiF = ipv4.Assign (dCdF);
	
   ipv4.SetBase ("10.1.4.0", "255.255.255.0");
   Ipv4InterfaceContainer iDiG = ipv4.Assign (dDdG);
   
   ipv4.SetBase ("10.1.5.0", "255.255.255.0");
   Ipv4InterfaceContainer iEiG = ipv4.Assign (dEdG);
 
   ipv4.SetBase ("10.1.6.0", "255.255.255.0");
   Ipv4InterfaceContainer iFiG = ipv4.Assign (dFdG);
 
   ipv4.SetBase ("10.1.7.0", "255.255.255.0");
   Ipv4InterfaceContainer iGiS = ipv4.Assign (dGdS);
	
   ipv4.SetBase ("10.1.8.0", "255.255.255.0");
   Ipv4InterfaceContainer iGiR = ipv4.Assign (dGdR);
	
	
   // Create router nodes, initialize routing database and set up the routing
   // tables in the nodes.
   Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
   
   
     NS_LOG_INFO ("Create Applications.");
	//
	// Create a UdpServer application on node Server (S).
	//
    uint16_t port_number = 9;  
   ApplicationContainer server_apps;
   UdpServerHelper serverS (port_number);
   server_apps.Add(serverS.Install(c.Get (7)));
   
   Ptr<UdpServer> S1 = serverS.GetServer();
   
      
   // We Initialize the sockets responsable for transmitting messages
  
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
   
   //Transmission Server (S)-> Router (R)
  Ptr<Socket> source1 = Socket::CreateSocket (c.Get (7), tid);
  InetSocketAddress remote1 = InetSocketAddress (iGiR.GetAddress (1), port_number);
  source1->Connect (remote1);
  
  //Transmission Server (S) -> Client (A or B or C or D or E or F)
  Ptr<Socket> source2 = Socket::CreateSocket (c.Get (7), tid);
   
   S1->TraceConnectWithoutContext ("RxWithAddresses", MakeBoundCallback (&received_msg, source1, source2));
   
   server_apps.Start (Seconds (1.0));
   server_apps.Stop (Seconds (10.0));
   
   	//
	// Create a UdpServer application on node A,B,C,D to receive the reply from the server.
	//
   UdpServerHelper server (port_number);
   server_apps.Add(server.Install(c.Get (0)));
   server_apps.Add(server.Install(c.Get (1)));
   server_apps.Add(server.Install(c.Get (2)));
   server_apps.Add(server.Install(c.Get (3)));

   
   // ####Using Sockets to generate traffic at node A, B, C, D, 
   //(i.e., exponential payload and inter-transmission time)####
   // You can in alternative install two Udp Client applications 
  	
  Ptr<Socket> sourceA = Socket::CreateSocket (c.Get (0), tid);
  InetSocketAddress remote = InetSocketAddress (iGiS.GetAddress (1), port_number);
  sourceA->Connect (remote);
  
  Ptr<Socket> sourceB= Socket::CreateSocket (c.Get (1), tid);
  sourceB->Connect (remote);
  
  Ptr<Socket> sourceC= Socket::CreateSocket (c.Get (2), tid);
  sourceC->Connect (remote);
  
  Ptr<Socket> sourceD= Socket::CreateSocket (c.Get (3), tid);
  sourceD->Connect (remote);
  
  //Ptr<Socket> sourceE= Socket::CreateSocket (c.Get (4), tid);
  //sourceE->Connect (remote);
  
  //Ptr<Socket> sourceF= Socket::CreateSocket (c.Get (5), tid);
  //sourceF->Connect (remote);
  
  
   //Mean inter-transmission time: A,B:2ms; C:0.5ms; D:1ms.
   double mean_AB = 0.002; //2 ms
   double mean_C = 0.0005; //0.5 ms
   double mean_D = 0.001; //1 ms
   Ptr<ExponentialRandomVariable> randomTime_AB = CreateObject<ExponentialRandomVariable> ();
   randomTime_AB->SetAttribute ("Mean", DoubleValue (mean_AB));
   Ptr<ExponentialRandomVariable> randomTime_C = CreateObject<ExponentialRandomVariable> ();
   randomTime_C->SetAttribute ("Mean", DoubleValue (mean_C));
   Ptr<ExponentialRandomVariable> randomTime_D = CreateObject<ExponentialRandomVariable> ();
   randomTime_D->SetAttribute ("Mean", DoubleValue (mean_D));
   
   //Mean packet time
   double mean = 100; // 100 Bytes
   Ptr<ExponentialRandomVariable> randomSize = CreateObject<ExponentialRandomVariable> ();
   randomSize->SetAttribute ("Mean", DoubleValue (mean));

   Simulator::ScheduleWithContext (sourceA->GetNode ()->GetId (), Seconds (2.0), &GenerateTraffic, sourceA, randomSize, randomTime_AB);
   Simulator::ScheduleWithContext (sourceB->GetNode ()->GetId (), Seconds (2.0), &GenerateTraffic, sourceB, randomSize, randomTime_AB);
   Simulator::ScheduleWithContext (sourceC->GetNode ()->GetId (), Seconds (2.0), &GenerateTraffic, sourceC, randomSize, randomTime_C);
   Simulator::ScheduleWithContext (sourceD->GetNode ()->GetId (), Seconds (2.0), &GenerateTraffic, sourceD, randomSize, randomTime_D);


 
   AsciiTraceHelper ascii;
   p2p_1.EnableAsciiAll (ascii.CreateFileStream ("projectA-1.tr"));
   p2p_1.EnablePcapAll ("projectA-1");
   p2p_2.EnableAsciiAll (ascii.CreateFileStream ("projectA-2.tr"));
   p2p_2.EnablePcapAll ("projectA-2");
   p2p_3.EnableAsciiAll (ascii.CreateFileStream ("projectA-3.tr"));
   p2p_3.EnablePcapAll ("projectA-3");
 
   AnimationInterface anim ("project.xml");
   anim.EnablePacketMetadata (true);
  anim.SetConstantPosition (c.Get(0), 100, -100);
  anim.SetConstantPosition (c.Get(1), 120, -100);
  anim.SetConstantPosition (c.Get(2), 140, -90);
  anim.SetConstantPosition (c.Get(3), 100, -90);
  anim.SetConstantPosition (c.Get(4), 100, -80);
  anim.SetConstantPosition (c.Get(5), 120, -90);
  anim.SetConstantPosition (c.Get(6), 110, -80);//
  anim.SetConstantPosition (c.Get(7), 50, -80);
  anim.SetConstantPosition (c.Get(8), 110, -70);

  
  
   // Flow Monitor
   FlowMonitorHelper flowmonHelper;
   if (enableFlowMonitor)
     {
       flowmonHelper.InstallAll ();
     }
 
   NS_LOG_INFO ("Run Simulation.");
   Simulator::Stop (Seconds (12));
   Simulator::Run ();
   NS_LOG_INFO ("Done.");
 
   if (enableFlowMonitor)
     {
       flowmonHelper.SerializeToXmlFile ("project.flowmon", false, false);
     }
 
   Simulator::Destroy ();
   return 0;
 }
