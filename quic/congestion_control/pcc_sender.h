#ifndef NET_QUIC_CONGESTION_CONTROL_PCC_SENDER_H_
#define NET_QUIC_CONGESTION_CONTROL_PCC_SENDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/net_export.h"
#include "net/quic/congestion_control/send_algorithm_interface.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_connection_stats.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

class RttStats;

class NET_EXPORT_PRIVATE PCCSender : public SendAlgorithmInterface {
 public:
  PCCSender();
  ~PCCSender() override;

  // SendAlgorithmInterface methods.
  void SetFromConfig(const QuicConfig& config,
                     Perspective perspective) override;
  void ResumeConnectionState(
      const CachedNetworkParameters& cached_network_params,
      bool max_bandwidth_resumption) override;
  void SetNumEmulatedConnections(int num_connections) override;
  void SetMaxCongestionWindow(QuicByteCount max_congestion_window) override;
  void OnCongestionEvent(bool rtt_updated,
                         QuicByteCount bytes_in_flight,
                         const CongestionVector& acked_packets,
                         const CongestionVector& lost_packets) override;
  bool OnPacketSent(QuicTime sent_time,
                    QuicByteCount bytes_in_flight,
                    QuicPacketNumber packet_number,
                    QuicByteCount bytes,
                    HasRetransmittableData is_retransmittable) override;
  void OnRetransmissionTimeout(bool packets_retransmitted) override;
  QuicTime::Delta TimeUntilSend(
      QuicTime now,
      QuicByteCount bytes_in_flight,
      HasRetransmittableData has_retransmittable_data) const override;
  QuicBandwidth PacingRate() const override;
  QuicBandwidth BandwidthEstimate() const override;
  QuicTime::Delta RetransmissionDelay() const override;
  QuicByteCount GetCongestionWindow() const override;
  bool InSlowStart() const override;
  bool InRecovery() const override;
  QuicByteCount GetSlowStartThreshold() const override;
  CongestionControlType GetCongestionControlType() const override;
  // End implementation of SendAlgorithmInterface.

 private:
 	const QuicTime::Delta alarm_granularity_ = QuicTime::Delta::FromMilliseconds(1);
  DISALLOW_COPY_AND_ASSIGN(PCCSender);
};

}  // namespace net

#endif
