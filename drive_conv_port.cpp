
/**
 * \file drive_conv_port.cpp
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
  *             Port conversion driver .cpp
  * ********************************************************
  * This modulus converts signals from the discrete domain to the continuous domain (analog)
  *
  */

#include "drive_conv_port.h"
#include <systemc.h>

/**
 * \fn void drive_conv_port::set_attributes()
 * \brief Determines the time in which the sample is read and written
 */
void drive_conv_port::set_attributes() {
    set_timestep(Tm);
}


/**
 * \fn void drive_conv_port::processing()
 *
 * \brief this method read the value of discrete domain entries and
 * And converts to the analog domain
 */
void drive_conv_port::processing(){

        if (sampcon_de_por_0.read() == true){

            sampcon_tdf_port.write(true);
            sampcon_de_port_out.write(true);

        }else if (sampcon_de_por_0.read() == false){

            sampcon_tdf_port.write(false);
            sampcon_de_port_out.write(false);

        }
}
