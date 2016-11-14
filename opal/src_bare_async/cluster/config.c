/*
 * OPAL (OpenCL Processor Array LBM solver)
 *
 * Copyright (c) 2014, Christian Obrecht <courriel@obrecht.fr>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/*******************************************************************************
 *
 *  config.c: Config structure creation and destruction functions.
 *
 */

// #include <string.h>
// #include <jansson.h>
#include "opal.h"

/*
 *  getConfig: read configuration file in JSON format and retrieve values of
 *  configuration parameters.
 */

// config* getConfig(int argc, char** argv)
config getConfig()
{
    config Config;

#if 1
    // Config = malloc(sizeof(config));
    // if (Config == NULL)
    //     printf("Failed to allocate memory for 'Config' structure.\n");
    
    Config.Duration = DURATION;
    #if MONITORING == 1
    Config.Period = DURATION/10;
    Config.Images = 1;
    #else
    Config.Period = DURATION;
    Config.Images = 0;
    #endif
    Config.Lx = CAVITY;
    Config.Ly = CAVITY;
    Config.Lz = CAVITY;
    Config.Nx = 0;
    Config.Ny = 0;
    Config.Nz = 0;
    Config.Re = 1000.0f;
    Config.U0 = 0.4f;
    Config.epsilon = 0.00001f;

#else
    json_t* root;
    json_error_t error;
    json_t* Path;
    json_t* Prefix;
    json_t* Log;
    json_t* Images;
    json_t* Export;
    json_t* Lx;
    json_t* Ly;
    json_t* Lz;
    json_t* Nx;
    json_t* Ny;
    json_t* Nz;
    json_t* Duration;
    json_t* Period;
    json_t* Re;
    json_t* U0;
    json_t* epsilon;

    if (argc != 2) eprintf("Usage is: %s <configuration file>.\n", argv[0]);
    root = json_load_file(argv[1], 0, &error);
    if (root == NULL)
        eprintf("JSON error on line %d: %s", error.line, error.text);
    Config = malloc(sizeof(config));
    if (Config == NULL)
        eprintf("Failed to allocate memory for 'Config' structure.\n");

    // Path to destination directory
    Path = json_object_get(root, "Path");
    if (!json_is_string(Path))
        eprintf("Config: 'Path' is missing or not a string.\n");
    Config->Path = json_string_value(Path);
    if (strlen(Config->Path) == 0)
        eprintf("Config: 'Path' must not be empty.\n");

    // Prefix of generated files
    Prefix = json_object_get(root, "Prefix");
    if (!json_is_string(Prefix))
        eprintf("Config: 'Prefix' is missing or not a string.\n");
    Config->Prefix = json_string_value(Prefix);
    if (strlen(Config->Prefix) == 0)
        eprintf("Config: 'Prefix' must not be empty.\n");

    // Simulation duration (in time steps)
    Duration = json_object_get(root, "Duration");
    if (!json_is_integer(Duration))
        eprintf("Config: 'Duration' is missing or not an integer.\n");
    Config->Duration = (int)json_integer_value(Duration);
    if (Config->Duration <= 0)
        eprintf("Config: 'Duration' must be positive.\n");
    if (Config->Duration & 1)
        eprintf("Config: 'Duration' must be even.\n");

    // Monitoring period (in time steps)
    Period = json_object_get(root, "Period");
    if (!json_is_integer(Period))
        eprintf("Config: 'Period' is missing or not an integer.\n");
    Config->Period = (int)json_integer_value(Period);
    if (Config->Period <= 0)
        eprintf("Config: 'Period' must be positive.\n");
    if (Config->Period & 1)
        eprintf("Config: 'Period' must be even.\n");

    // Computation log flag
    Log = json_object_get(root, "Log");
    if (!json_is_boolean(Log))
        eprintf("Config: 'Log' is missing or not a boolean.\n");
    Config->Log = (bool)json_is_true(Log);

    // Graphic monitoring flag
    Images = json_object_get(root, "Images");
    if (!json_is_boolean(Images))
        eprintf("Config: 'Images' is missing or not a boolean.\n");
    Config->Images = (bool)json_is_true(Images);

    // Result exportation flag
    Export = json_object_get(root, "Export");
    if (!json_is_boolean(Export))
        eprintf("Config: 'Export' is missing or not a boolean.\n");
    Config->Export = (bool)json_is_true(Export);

    // Domain size in x-direction (in nodes)
    // We impose 'Lx' to be a multiple of 4 to avoid messing up our simplistic
    // BMP creation function.
    Lx = json_object_get(root, "Lx");
    if (!json_is_integer(Lx))
        eprintf("Config: 'Lx' is missing or not an integer.\n");
    Config->Lx = (int)json_integer_value(Lx);
    if (Config->Lx <= 0)
        eprintf("Config: 'Lx' must be positive.\n");
    if (Config->Lx % 4 != 0)
        eprintf("Config: 'Lx' must be at least a multiple of 4.\n");

    // Domain size in y-direction (in nodes)
    Ly = json_object_get(root, "Ly");
    if (!json_is_integer(Ly))
        eprintf("Config: 'Ly' is missing or not an integer.\n");
    Config->Ly = (int)json_integer_value(Ly);
    if (Config->Ly <= 0)
        eprintf("Config: 'Ly' must be positive.\n");

    // Domain size in z-direction (in nodes)
    Lz = json_object_get(root, "Lz");
    if (!json_is_integer(Lz))
        eprintf("Config: 'Lz' is missing or not an integer.\n");
    Config->Lz = (int)json_integer_value(Lz);
    if (Config->Lz <= 0)
        eprintf("Config: 'Lz' must be positive.\n");

    // Workgroup size in x-direction (in nodes)
    Nx = json_object_get(root, "Nx");
    if (!json_is_integer(Nx))
        eprintf("Config: 'Nx' is missing or not an integer.\n");
    Config->Nx = (int)json_integer_value(Nx);
    if (Config->Nx <= 0)
        eprintf("Config: 'Nx' must be positive.\n");
    if (Config->Lx % Config->Nx != 0)
        eprintf("Config: 'Nx' must be a divisor of 'Lx'.\n");

    // Workgroup size in y-direction (in nodes)
    Ny = json_object_get(root, "Ny");
    if (!json_is_integer(Ny))
        eprintf("Config: 'Ny' is missing or not an integer.\n");
    Config->Ny = (int)json_integer_value(Ny);
    if (Config->Ny <= 0)
        eprintf("Config: 'Ny' must be positive.\n");
    if (Config->Ly % Config->Ny != 0)
        eprintf("Config: 'Ny' must be a divisor of 'Ly'.\n");

    // Workgroup size in z-direction (in nodes)
    Nz = json_object_get(root, "Nz");
    if (!json_is_integer(Nz))
        eprintf("Config: 'Nz' is missing or not an integer.\n");
    Config->Nz = (int)json_integer_value(Nz);
    if (Config->Nz <= 0)
        eprintf("Config: 'Nz' must be positive.\n");
    if (Config->Lz % Config->Nz != 0)
        eprintf("Config: 'Nz' must be a divisor of 'Lz'.\n");

    // Reynolds number
    Re = json_object_get(root, "Re");
    if (!json_is_real(Re))
        eprintf("Config: 'Re' is missing or not a real value.\n");
    Config->Re = (float)json_real_value(Re);
    if (Config->Re <= 0)
        eprintf("Config: 'Re' must be positive.\n");

    // Imposed velocity
    U0 = json_object_get(root, "U0");
    if (!json_is_real(U0))
        eprintf("Config: 'U0' is missing or not a real value.\n");
    Config->U0 = (float)json_real_value(U0);
    if (Config->U0 <= 0)
       eprintf("Config: 'U0' must be positive.\n");

    // Convergence threshold
    epsilon = json_object_get(root, "epsilon");
    if (!json_is_real(epsilon))
        eprintf("Config: 'epsilon' is missing or not a real value.\n");
    Config->epsilon = (float)json_real_value(epsilon);
#endif

    return Config;
}

