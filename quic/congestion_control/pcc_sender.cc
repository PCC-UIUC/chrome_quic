#include "net/quic/congestion_control/pcc_sender.h"

#include <stdio.h>

namespace net {

PCCSender::PCCSender()
	: ideal_next_packet_send_time_(QuicTime::Zero()),
		was_last_send_delayed_(false){
		printf("pcc\n");
}

PCCSender::~PCCSender() {}

void PCCSender::SetFromConfig(const QuicConfig& config,
                                 bool is_server,
                                 bool using_pacing) {
}

bool PCCSender::ResumeConnectionState(
    const CachedNetworkParameters& cached_network_params) {
  return true;
}

void PCCSender::SetNumEmulatedConnections(int num_connections) {
  
}

bool PCCSender::OnPacketSent(
    QuicTime sent_time,
    QuicByteCount bytes_in_flight,
    QuicPacketSequenceNumber sequence_number,
    QuicByteCount bytes,
    HasRetransmittableData has_retransmittable_data) {
  QuicTime zero = QuicTime::Zero();
  QuicTime::Delta time = sent_time.Subtract(zero);
  printf("\n");
  printf("sent\n");
  printf("sent time: %ld\n", time.ToMicroseconds());
  printf("bytes in flight: %lu\n", bytes_in_flight);
  printf("sequence number: %lu\n", sequence_number);
  printf("bytes: %lu\n", bytes);
  QuicTime::Delta delay = QuicTime::Delta::FromMilliseconds(10);
  if(was_last_send_delayed_){
  	was_last_send_delayed_ = false;
  	ideal_next_packet_send_time_ = ideal_next_packet_send_time_.Add(delay);
  }
  else{
    ideal_next_packet_send_time_ = QuicTime::Max(
        ideal_next_packet_send_time_.Add(delay), sent_time.Add(delay));
  }
  return true;
}

void PCCSender::OnCongestionEvent(
    bool rtt_updated,
    QuicByteCount bytes_in_flight,
    const CongestionVector& acked_packets,
    const CongestionVector& lost_packets) {
  printf("\n");
  printf("event\n");
  printf("rtt updated: %d\n", rtt_updated);
  printf("bytes in flight: %lu\n", bytes_in_flight);
  int size = acked_packets.size();
  printf("acked packets\n");
  for(int i = 0; i < size; i++){
    printf("sequence Number:%lu \n", (QuicPacketSequenceNumber)acked_packets[i].first);
  }
  size = lost_packets.size();
  printf("lost packets\n");
  for(int i = 0; i < size; i++){
    printf("Sequence Number:%lu \n", (QuicPacketSequenceNumber)lost_packets[i].first);
  }
}

void PCCSender::OnRetransmissionTimeout(bool packets_retransmitted) {

}

void PCCSender::RevertRetransmissionTimeout() {

}

QuicTime::Delta PCCSender::TimeUntilSend(
      QuicTime now,
      QuicByteCount bytes_in_flight,
      HasRetransmittableData has_retransmittable_data) const {
  //printf("pcc pacing\n");
  if (ideal_next_packet_send_time_ > now.Add(alarm_granularity_)) {
    //DVLOG(1) << "Delaying packet: "
    //         << ideal_next_packet_send_time_.Subtract(now).ToMicroseconds();
    was_last_send_delayed_ = true;
    return ideal_next_packet_send_time_.Subtract(now);
  }
  return QuicTime::Delta::Zero();
}

QuicBandwidth PCCSender::PacingRate() const {
  return QuicBandwidth::Zero();
}

QuicBandwidth PCCSender::BandwidthEstimate() const {
  return QuicBandwidth::Zero();
}

bool PCCSender::HasReliableBandwidthEstimate() const {
  return false;
}

QuicTime::Delta PCCSender::RetransmissionDelay() const {
  return QuicTime::Delta::Zero();
}

QuicByteCount PCCSender::GetCongestionWindow() const {
  return 1000*kMaxPacketSize;
}

bool PCCSender::InSlowStart() const {
  return false;
}

bool PCCSender::InRecovery() const {
  return false;
}

QuicByteCount PCCSender::GetSlowStartThreshold() const {
  return 1000*kMaxPacketSize;
}

CongestionControlType PCCSender::GetCongestionControlType() const {
  return kPcc;
}

}  // namespace net

