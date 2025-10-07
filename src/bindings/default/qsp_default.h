/* Copyright (C) 2005-2010 Valeriy Argunov (nporep AT mail DOT ru) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef QSP_DEFAULTDEFINES
	#define QSP_DEFAULTDEFINES

	#ifdef _UNICODE
		typedef wchar_t QSP_CHAR;
		#define QSP_FMT2(x) L##x
		#define QSP_FMT(x) QSP_FMT2(x)

		#define QSP_STRCOLL qspStrsComp
		#define QSP_CHRLWR qspToWLower
		#define QSP_CHRUPR qspToWUpper
		#define QSP_WCSTOMBSLEN(a) (int)wcstombs(0, a, 0)
		#define QSP_WCSTOMBS wcstombs
		#define QSP_MBTOSB(a) ((a) % 256)
		#define QSP_ONIG_ENC (sizeof(QSP_CHAR) == 2 ? ONIG_ENCODING_UTF16_LE : ONIG_ENCODING_UTF32_LE)
		#define QSP_FROM_OS_CHAR(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
		#define QSP_TO_OS_CHAR(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
		#define QSP_WCTOB
		#define QSP_BTOWC

		#ifdef _MSC_VER
			#define QSP_FOPEN _wfopen
			#define QSP_FDOPEN _wfdopen
		#else
			#define QSP_FOPEN qspFileOpen
			#define QSP_FDOPEN qspFileDescOpen
		#endif
	#else
		typedef char QSP_CHAR;
		#define QSP_FMT(x) x

		#define QSP_FOPEN fopen
		#define QSP_WCSTOMBSLEN qspStrLen
		#define QSP_WCSTOMBS qspStrNCopy
		#define QSP_MBTOSB(a) ((unsigned char)(a))
		#if defined(_WIN) || defined(_PSP)
			#define QSP_FROM_OS_CHAR
			#define QSP_TO_OS_CHAR
			#define QSP_WCTOB(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
			#define QSP_BTOWC(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
			#define QSP_CHRLWR(a) qspCP1251ToLowerTable[(unsigned char)(a)]
			#define QSP_CHRUPR(a) qspCP1251ToUpperTable[(unsigned char)(a)]
			#define QSP_STRCOLL(a, b) qspStrCmpSB(a, b, qspCP1251OrderTable)
			#define QSP_ONIG_ENC ONIG_ENCODING_CP1251
		#else
			#define QSP_FROM_OS_CHAR(a) qspReverseConvertSB(a, qspCP1251ToKOI8RTable)
			#define QSP_TO_OS_CHAR(a) qspDirectConvertSB(a, qspCP1251ToKOI8RTable)
			#define QSP_WCTOB(a) qspReverseConvertUC(a, qspKOI8RToUnicodeTable)
			#define QSP_BTOWC(a) qspDirectConvertUC(a, qspKOI8RToUnicodeTable)
			#define QSP_CHRLWR(a) qspKOI8RToLowerTable[(unsigned char)(a)]
			#define QSP_CHRUPR(a) qspKOI8RToUpperTable[(unsigned char)(a)]
			#define QSP_STRCOLL(a, b) qspStrCmpSB(a, b, qspKOI8ROrderTable)
			#define QSP_ONIG_ENC ONIG_ENCODING_KOI8_R
		#endif
	#endif

	#ifdef _MSC_VER
		#define QSP_TIME _time64
	#else
		#define QSP_TIME time
	#endif

	#ifdef __cplusplus
		typedef int (*QSP_CALLBACK)(...);
	#else
		typedef int (*QSP_CALLBACK)();
	#endif

	#include "../qsp.h"

	#ifdef __cplusplus
	extern "C"
	{
	#endif

	QSLIBSDH_EXTERN QSP_BOOL QSPIsInCallBack();
	QSLIBSDH_EXTERN void QSPEnableDebugMode(QSP_BOOL isDebug);
	QSLIBSDH_EXTERN void QSPGetCurStateData(QSP_CHAR **loc, int *actIndex, int *line);
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetVersion();
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetCompiledDateTime();
	QSLIBSDH_EXTERN int QSPGetFullRefreshCount();
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetQstFullPath();
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetCurLoc();
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetMainDesc();
	QSLIBSDH_EXTERN QSP_BOOL QSPIsMainDescChanged();
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetVarsDesc();
	QSLIBSDH_EXTERN QSP_BOOL QSPIsVarsDescChanged();
	QSLIBSDH_EXTERN QSP_BOOL QSPGetExprValue(const QSP_CHAR *str, QSP_BOOL *isString, int *numVal, QSP_CHAR *strVal, int strValBufSize);
	QSLIBSDH_EXTERN void QSPSetInputStrText(const QSP_CHAR *str);
	QSLIBSDH_EXTERN int QSPGetActionsCount();
	QSLIBSDH_EXTERN void QSPGetActionData(int ind, QSP_CHAR **imgPath, QSP_CHAR **desc);
	QSLIBSDH_EXTERN QSP_BOOL QSPExecuteSelActionCode(QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPSetSelActionIndex(int ind, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN int QSPGetSelActionIndex();
	QSLIBSDH_EXTERN QSP_BOOL QSPIsActionsChanged();
	QSLIBSDH_EXTERN int QSPGetObjectsCount();
	QSLIBSDH_EXTERN void QSPGetObjectData(int ind, QSP_CHAR **imgPath, QSP_CHAR **desc);
	QSLIBSDH_EXTERN QSP_BOOL QSPSetSelObjectIndex(int ind, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN int QSPGetSelObjectIndex();
	QSLIBSDH_EXTERN QSP_BOOL QSPIsObjectsChanged();
	QSLIBSDH_EXTERN void QSPShowWindow(int type, QSP_BOOL isShow);
	QSLIBSDH_EXTERN QSP_BOOL QSPGetVarValuesCount(const QSP_CHAR *name, int *count);
	QSLIBSDH_EXTERN QSP_BOOL QSPGetVarValues(const QSP_CHAR *name, int ind, int *numVal, QSP_CHAR **strVal);
	QSLIBSDH_EXTERN int QSPGetMaxVarsCount();
	QSLIBSDH_EXTERN QSP_BOOL QSPGetVarNameByIndex(int ind, QSP_CHAR **name);
	QSLIBSDH_EXTERN QSP_BOOL QSPExecString(const QSP_CHAR *str, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPExecCounter(QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPExecLocationCode(const QSP_CHAR *name, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN void QSPGetLastErrorData(int *errorNum, QSP_CHAR **errorLoc, int *errorActIndex, int *errorLine);
	QSLIBSDH_EXTERN const QSP_CHAR *QSPGetErrorDesc(int errorNum);
	QSLIBSDH_EXTERN QSP_BOOL QSPLoadGameWorld(const QSP_CHAR *file);
	QSLIBSDH_EXTERN QSP_BOOL QSPLoadGameWorldFromData(const char *data, int dataSize, const QSP_CHAR *file);
	QSLIBSDH_EXTERN QSP_BOOL QSPSaveGame(const QSP_CHAR *file, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPSaveGameAsString(QSP_CHAR *strBuf, int strBufSize, int *realSize, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPOpenSavedGame(const QSP_CHAR *file, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPOpenSavedGameFromString(const QSP_CHAR *str, QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN QSP_BOOL QSPRestartGame(QSP_BOOL isRefresh);
	QSLIBSDH_EXTERN void QSPSelectMenuItem(int ind);
	QSLIBSDH_EXTERN void QSPSetCallBack(int type, QSP_CALLBACK func);
	QSLIBSDH_EXTERN void QSPInit();
	QSLIBSDH_EXTERN void QSPDeInit();

	#ifdef __cplusplus
	}
	#endif

#endif
