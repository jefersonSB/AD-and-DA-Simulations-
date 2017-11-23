/**
 * \file drive_conv_port.h
 * \brief drive_conv_port file.
 *
 * \author Jeferson Santos Barros <jeferson.s.barros@posgrad.ufsc.br>
 *
 * \version 1.0-dev
 *
 * \date 03/07/2017
 *
 * \defgroup main Main
 * \{
 */

/**
  * ********************************************************
  *             Port conversion driver .h
  * ********************************************************
  * This modulus converts signals from the discrete domain to the continuous domain (analog)
  *
  */

#ifndef DRIVE_CONV_PORT_H
#define DRIVE_CONV_PORT_H

//#include "/opt/systemc-ams/include/systemc-ams.h"
#include <systemc-ams>

/**
  * \class SCA_TDF_MODULE (drive_conv_port)
  * \brief This class converts the SAMPCON signal that keys the sample end hold
  *  from the discrete domain to the analog domain.
  */

SCA_TDF_MODULE(drive_conv_port){

    sca_tdf::sca_out < bool >  sampcon_tdf_port;        // Signal output port for switching sample and hold
    sca_tdf::sc_out  < bool >  sampcon_de_port_out;     // Signal output port from Sar triguer conversion
    sca_tdf::sc_in   < bool >  sampcon_de_por_0;        // Switching signal input port from clk

    void set_attributes();                              ///< \fn Defines attributes related to the TDF model
    void processing();                                  ///< \fn Decreases behavior in the time domain


    drive_conv_port( sc_core::sc_module_name nm, sca_core::sca_time Tm_)
          : Tm (Tm_)
    {}

    private:
        sca_core::sca_time Tm; // module time step
};
#endif
