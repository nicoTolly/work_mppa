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
 *  main.c: main program function.
 *
 */

#include "opal.h"

int main(int argc, char **argv)
{
    config* Config;
    frame* Frame;
    data* Data;
    app* App;
    results* Results;
    timespec Start, Stop;
    int Time;

    // Start timer
    clock_gettime(CLOCK_REALTIME, &Start);
    // Create containers
    Config = getConfig(argc, argv);
    Frame = createFrame(Config);
    Data = createData(Config, Frame);
    App = createApp(Config, Frame, Data);
    Results = createResults(Config);
    // Begin log
    printBanner(Config, Results);

    // Initialise simulation
    launchInitKernel(Frame, App, Data, Results);
    printResults(Results, 0);
    // Optional monitoring image
    if (Config->Images){
	   exportImage(Config, Results, 0);
    }

    // Main computation loop
    for (Time = 0; Time <= Config->Duration && !(Results->Converged); Time++) {
        launchComputeKernel(Frame, App, Time);
	// Monitoring
        if (Time > 0 && Time % Config->Period == 0) {
            launchMonitorKernel(Frame, App, Data, Results);
            printResults(Results, Time);
	    // Optional monitoring image
            if (Config->Images){
                exportImage(Config, Results, Time);
            }
        }
    }
    // Optional simulation results exportation
    if (Config->Export) {
        launchExtractKernel(Frame, App, Data, Results);
        exportResults(Results);
    }

    // Clear containers
    clearApp(App);
    clearData(Data);
    clearFrame(Frame);
    free(Config);

    // Stop timer
    clock_gettime(CLOCK_REALTIME, &Stop);
    // Finalise log
    printSummary(Results, Time, &Start, &Stop);
    clearResults(Results);

    return EXIT_SUCCESS;
}
