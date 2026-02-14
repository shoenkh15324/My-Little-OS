#pragma once
/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
#include "core/sysDefs.h"
#include "core/config/platformConfig.h"
#include "core/feature/log.h"
#include "core/feature/buffer.h"
#include "core/feature/async.h"
#include "core/feature/osal.h"

int driverCommonClose(void);
int driverCommonOpen(void);
