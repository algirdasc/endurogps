#include <ublox/ubxNMEA.h>

#define LAST_SENTENCE_IN_INTERVAL (NMEAGPS::nmea_msg_t) ubloxNMEA::PUBX_00

#if !defined( NMEAGPS_PARSE_GGA ) & !defined( NMEAGPS_PARSE_GLL ) & \
    !defined( NMEAGPS_PARSE_GSA ) & !defined( NMEAGPS_PARSE_GSV ) & \
    !defined( NMEAGPS_PARSE_RMC ) & !defined( NMEAGPS_PARSE_VTG ) & \
    !defined( NMEAGPS_PARSE_ZDA ) & !defined( NMEAGPS_PARSE_GST ) & \
    !defined( NMEAGPS_PARSE_PUBX_00 ) & !defined( NMEAGPS_PARSE_PUBX_04 )

  #error No NMEA sentences enabled: no fix data available.

#endif

#if !defined( NMEAGPS_PARSE_PUBX_00 ) & !defined( NMEAGPS_PARSE_PUBX_04 )
  #error No PUBX messages enabled!
#endif

#ifndef NMEAGPS_DERIVED_TYPES
  #error You must "#define NMEAGPS_DERIVED_TYPES" in NMEAGPS_cfg.h!
#endif

#ifndef NMEAGPS_EXPLICIT_MERGING
  #error You must define NMEAGPS_EXPLICIT_MERGING in NMEAGPS_cfg.h
#endif

#ifdef NMEAGPS_INTERRUPT_PROCESSING
  #error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif