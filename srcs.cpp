


# include"srcs.h"

void srcs::set_attributes() {
    set_timestep(Tm);
}

void srcs::processing() {

    t = get_time().to_seconds(); // actual time
    out_tdf_de.write((ampl) * sin( (2.0 * M_PI * freq * t))+ampl);
}
