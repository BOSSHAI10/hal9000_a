#pragma once

void
CmdListNetworks(
    IN          QWORD       NumberOfParameters
    );

void
CmdNetRecv(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       ResendString
    );

void
CmdNetSend(
    IN          QWORD       NumberOfParameters
   );

void
CmdChangeDevStatus(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       DeviceString,
    IN_Z    char*       RxEnableString,
    IN_Z    char*       TxEnableString
    );
