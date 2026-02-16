#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
#include "appCfgSelector.h"

#include "core/systemDefs.h"
#include "driverDefs.h"
#include "core/feature/log.h"
#include "core/feature/buffer.h"
#include "core/feature/async.h"
#include "core/feature/osal.h"

int driverCommonClose(void);
int driverCommonOpen(void);
