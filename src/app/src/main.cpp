// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#include "app.h"

using namespace Arrival::App;

// Main entry point.
int main(int argc, char *argv[])
{
    ArrivalApp app(argc, argv);
    return app.exec();
}
