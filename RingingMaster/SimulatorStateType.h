#pragma once

enum SimulatorStateType
{
	sst_disconnected, //0
	sst_connectedSilent, //1
	sst_connectedAutoStart, //2
	sst_connectedRoundsPendingFromSilent, //3
	sst_connectedRounds, //4
	sst_connectedMethodPendingHandFromRounds,//5
	sst_connectedMethodPendingBackFromRounds,//6
	sst_connectedMethod,//7
	sst_connectedRoundsPendingFromMethod, //8
	sst_connectedSilentPendingFromMethod, //9
	sst_connectedSilentPendingFromRounds, //10
};