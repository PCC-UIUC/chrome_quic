#ifndef NET_QUIC_CONGESTION_CONTROL_PCC_SENDER_H_
#define NET_QUIC_CONGESTION_CONTROL_PCC_SENDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/net_export.h"
#include "net/quic/congestion_control/cubic.h"
#include "net/quic/congestion_control/hybrid_slow_start.h"
#include "net/quic/congestion_control/prr_sender.h"
#include "net/quic/congestion_control/send_algorithm_interface.h"
#include "net/quic/crypto/cached_network_parameters.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_connection_stats.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

struct pcc_monitor {
  int packet_left;
  // for state, 1=sending, 2= waiting, 3=finished
  int state;

  // time statics  
  double start_time;
  double end_time;
  double end_transmission_time;

  // packet statics
  int total;
  int ack;
  int lost;
  int retransmission;
  int new_transmission;

  // latency statics
  int32_t latency;
  int32_t latency_seq_start;
  int32_t latency_seq_end;
  int32_t latency_time_start;
  int32_t latency_time_end;
  int32_t time_interval;
};

class RttStats;

class NET_EXPORT_PRIVATE PCCSender : public SendAlgorithmInterface {
 public:
  PCCSender();
  ~PCCSender() override;

  // SendAlgorithmInterface methods.
  void SetFromConfig(const QuicConfig& config,
                     bool is_server,
                     bool using_pacing) override;
  bool ResumeConnectionState(
      const CachedNetworkParameters& cached_network_params) override;
  void SetNumEmulatedConnections(int num_connections) override;
  void OnCongestionEvent(bool rtt_updated,
                         QuicByteCount bytes_in_flight,
                         const CongestionVector& acked_packets,
                         const CongestionVector& lost_packets) override;
  bool OnPacketSent(QuicTime sent_time,
                    QuicByteCount bytes_in_flight,
                    QuicPacketSequenceNumber sequence_number,
                    QuicByteCount bytes,
                    HasRetransmittableData is_retransmittable) override;
  void OnRetransmissionTimeout(bool packets_retransmitted) override;
  void RevertRetransmissionTimeout() override;
  QuicTime::Delta TimeUntilSend(
      QuicTime now,
      QuicByteCount bytes_in_flight,
      HasRetransmittableData has_retransmittable_data) const override;
  QuicBandwidth PacingRate() const override;
  QuicBandwidth BandwidthEstimate() const override;
  bool HasReliableBandwidthEstimate() const override;
  QuicTime::Delta RetransmissionDelay() const override;
  QuicByteCount GetCongestionWindow() const override;
  bool InSlowStart() const override;
  bool InRecovery() const override;
  QuicByteCount GetSlowStartThreshold() const override;
  CongestionControlType GetCongestionControlType() const override;
  // End implementation of SendAlgorithmInterface.

 private:
 	const QuicTime::Delta alarm_granularity_ = QuicTime::Delta::FromMilliseconds(1);

  // PCC monitor variable
  int current_monitor;
  int previous_monitor;
  int monitor_left;

  pcc_monitor monitor[100];



  DISALLOW_COPY_AND_ASSIGN(PCCSender);
};

}  // namespace net

#endif


