// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Symbian Foundation License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @internalComponent
*/

class Exec
	{
public:
	static void WaitForAnyRequest();
	static RAllocator* Heap();
	static RAllocator* HeapSwitch(RAllocator*);
	static TTrapHandler* PushTrapFrame(TTrap*);
	static TTrap* PopTrapFrame();
	static CActiveScheduler* ActiveScheduler();
	static void SetActiveScheduler(CActiveScheduler*);
	static TTimerLockSpec LockPeriod();
	static TTrapHandler* TrapHandler();
	static TTrapHandler* SetTrapHandler(TTrapHandler*);
	static TInt LockedInc(TInt&);
	static TInt LockedDec(TInt&);
	static TUint32 DebugMask();
	static TUint32 DebugMaskIndex(TUint);
	static void SetDebugMask(TUint32);
	static TUint32 FastCounter();
	static TUint32 NTickCount();
	static TInt SafeInc(TInt&);
	static TInt SafeDec(TInt&);
	static void SetReentryPoint(TLinAddr);
	static TUint32 KernelConfigFlags();
	static TInt UTCOffset();
	static TInt GetGlobalUserData(TInt);
	static TInt ObjectNext(TObjectType, TBuf8<KMaxFullName>&, TFindHandle&);
	static TUint8* ChunkBase(TInt);
	static TInt ChunkSize(TInt);
	static TInt ChunkMaxSize(TInt);
	static TUint HandleAttributes(TInt);
	static TUint TickCount();
	static void LogicalDeviceGetCaps(TInt, TDes8&);
	static TBool LogicalDeviceQueryVersionSupported(TInt, const TVersion&);
	static TBool LogicalDeviceIsAvailable(TInt, TInt, const TDesC8*, const TDesC8*);
	static TInt ChannelRequest(TInt, TInt, TAny*, TAny*);
	static TUint32 MathRandom();
	static void IMB_Range(TAny*, TUint);
	static TInt ResetMachine(TMachineStartupType);
	static TLibraryFunction LibraryLookup(TInt, TInt);
	static void LibraryFileName(TInt, TDes8&);
	static void MutexWait(TInt);
	static void MutexSignal(TInt);
	static TInt ProcessId(TInt);
	static void DllFileName(TInt, TDes8&);
	static void ProcessResume(TInt);
	static void ProcessFileName(TInt, TDes8&);
	static void ProcessCommandLine(TInt, TDes8&);
	static TExitType ProcessExitType(TInt);
	static TInt ProcessExitReason(TInt);
	static void ProcessExitCategory(TInt, TDes8&);
	static TProcessPriority ProcessPriority(TInt);
	static void ProcessSetPriority(TInt, TProcessPriority);
	static TUint ProcessFlags(TInt);
	static void ProcessSetFlags(TInt, TUint, TUint);
	static TInt SemaphoreWait(TInt, TInt);
	static void SemaphoreSignal1(TInt);
	static void SemaphoreSignalN(TInt, TInt);
	static void ServerReceive(TInt, TRequestStatus&, TAny*);
	static void ServerCancel(TInt);
	static void SetSessionPtr(TInt, const TAny*);
	static TInt SessionSend(TInt, TInt, TAny*, TRequestStatus*);
	static TInt ThreadId(TInt);
	static TInt SessionShare(TInt&, TInt);
	static void ThreadResume(TInt);
	static void ThreadSuspend(TInt);
	static TThreadPriority ThreadPriority(TInt);
	static void ThreadSetPriority(TInt, TThreadPriority);
	static TProcessPriority ThreadProcessPriority(TInt);
	static void ThreadSetProcessPriority(TInt, TProcessPriority);
	static TUint ThreadFlags(TInt);
	static void ThreadSetFlags(TInt, TUint, TUint);
	static TInt ThreadRequestCount(TInt);
	static TExitType ThreadExitType(TInt);
	static TInt ThreadExitReason(TInt);
	static void ThreadExitCategory(TInt, TDes8&);
	static void TimerCancel(TInt);
	static void TimerAfter(TInt, TRequestStatus&, TInt);
	static void TimerAt(TInt, TRequestStatus&, const EXEC_TIME&);
	static void TimerLock(TInt, TRequestStatus&, TTimerLockSpec);
	static TInt ChangeNotifierLogon(TInt, TRequestStatus&);
	static TInt ChangeNotifierLogoff(TInt);
	static void RequestSignal(TInt);
	static void HandleName(TInt, TDes8&);
	static void HandleFullName(TInt, TDes8&);
	static void HandleInfo(TInt, THandleInfo*);
	static void HandleCount(TInt, TInt&, TInt&);
	static void After(TInt, TRequestStatus&);
	static void At(const EXEC_TIME&, TRequestStatus&);
	static void MessageComplete(TInt, TInt);
	static void MessageCompleteWithHandle(TInt, TInt);
	static TInt TimeNow(EXEC_TIME&, TInt&);
	static TInt TimeNowSecure(EXEC_TIME&, TInt&);
	static TInt SetUTCTimeAndOffset(const EXEC_TIME&, TInt, TUint, TUint);
	static TInt SetMachineConfiguration(const TDesC8&);
	static void CaptureEventHook();
	static void ReleaseEventHook();
	static void RequestEvent(TRawEventBuf&, TRequestStatus&);
	static void RequestEventCancel();
	static TInt AddEvent(const TRawEvent&);
	static TInt SessionSendSync(TInt, TInt, TAny*, TRequestStatus*);
	static TAny* DllTls(TInt, TInt);
	static TInt HalFunction(TInt, TInt, TAny*, TAny*);
	static void WsRegisterThread();
	static void FsRegisterThread();
	static TInt ProcessCommandLineLength(TInt);
	static void TimerInactivity(TInt, TRequestStatus&, TInt);
	static TInt UserInactivityTime();
	static void ResetInactivityTime();
	static void DebugPrint(TAny*, TInt);
	static TInt BreakPoint();
	static TInt ProfileStart(TInt);
	static TInt ProfileEnd(TInt);
	static TExceptionHandler ExceptionHandler(TInt);
	static TInt SetExceptionHandler(TInt, TExceptionHandler, TUint32);
	static void ModifyExceptionMask(TInt, TUint32, TUint32);
	static TInt RaiseException(TInt, TExcType);
	static TInt IsExceptionHandled(TInt, TExcType, TBool);
	static TInt ProcessGetMemoryInfo(TInt, TModuleMemoryInfo&);
	static TInt LibraryGetMemoryInfo(TInt, TModuleMemoryInfo&);
	static TInt MachineConfiguration(TDes8&, TInt&);
	static TInt SetMemoryThresholds(TInt, TInt);
	static void LibraryType(TInt, TUidType&);
	static void ProcessType(TInt, TUidType&);
	static TInt ChunkBottom(TInt);
	static TInt ChunkTop(TInt);
	static void ThreadContext(TInt, TDes8&);
	static TInt ThreadCreate(const TDesC8&, TOwnerType, SThreadCreateInfo8&);
	static TInt FindHandleOpen(TOwnerType, const TFindHandle&);
	static TInt HandleClose(TInt);
	static TInt ChunkCreate(TOwnerType, const TDesC8*, TChunkCreate&);
	static TInt ChunkAdjust(TInt, TInt, TInt, TInt);
	static TInt OpenObject(TObjectType, const TDesC8&, TOwnerType);
	static TInt HandleDuplicate(TInt, TOwnerType, TInt);
	static TInt MutexCreate(const TDesC8*, TOwnerType);
	static TInt SemaphoreCreate(const TDesC8*, TInt, TOwnerType);
	static TInt ThreadOpenById(TUint, TOwnerType);
	static TInt ProcessOpenById(TUint, TOwnerType);
	static void ThreadKill(TInt, TExitType, TInt, const TDesC8*);
	static void ThreadLogon(TInt, TRequestStatus*, TBool);
	static TInt ThreadLogonCancel(TInt, TRequestStatus*, TBool);
	static TInt DllSetTls(TInt, TInt, TAny*);
	static void DllFreeTls(TInt);
	static TInt ThreadRename(TInt, const TDesC8&);
	static TInt ProcessRename(TInt, const TDesC8&);
	static void ProcessKill(TInt, TExitType, TInt, const TDesC8*);
	static void ProcessLogon(TInt, TRequestStatus*, TBool);
	static TInt ProcessLogonCancel(TInt, TRequestStatus*, TBool);
	static TInt ThreadProcess(TInt);
	static TInt ServerCreate(const TDesC8*, TInt);
	static TInt SessionCreate(const TDesC8&, TInt, const TSecurityPolicy*, TInt);
	static TInt SessionCreateFromHandle(TInt, TInt, const TSecurityPolicy*, TInt);
	static TInt DeviceFree(const TDesC8&, TInt);
	static TInt ChannelCreate(const TDesC8&, TChannelCreateInfo8&, TInt);
	static TInt TimerCreate();
	static void TimerHighRes(TInt, TRequestStatus&, TInt);
	static void AfterHighRes(TInt, TRequestStatus&);
	static TInt ChangeNotifierCreate(TOwnerType);
	static TInt UndertakerCreate(TOwnerType);
	static TInt UndertakerLogon(TInt, TRequestStatus&, TInt&);
	static TInt UndertakerLogonCancel(TInt);
	static void KernelHeapDebug(TInt, TInt, TAny*);
	static TInt ThreadGetCpuTime(TInt, EXEC_INT64&);
	static TInt LastThreadHandle();
	static void ThreadRendezvous(TInt);
	static void ProcessRendezvous(TInt);
	static TInt MessageGetDesLength(TInt, TInt);
	static TInt MessageGetDesMaxLength(TInt, TInt);
	static TInt MessageIpcCopy(TInt, TInt, SIpcCopyInfo&, TInt);
	static TInt MessageClient(TInt, TOwnerType);
	static TInt MessageSetProcessPriority(TInt, TProcessPriority);
	static void MessageConstructFromPtr(TInt, TAny*);
	static void MessageKill(TInt, TExitType, TInt, const TDesC8*);
	static TInt MessageOpenObject(TInt, TObjectType, TInt, TOwnerType);
	static void ProcessSecurityInfo(TInt, SSecurityInfo&);
	static void ThreadSecurityInfo(TInt, SSecurityInfo&);
	static void MessageSecurityInfo(TInt, SSecurityInfo&);
	static void CreatorSecurityInfo(SSecurityInfo&);
	static void DisabledCapabilities(SCapabilitySet&);
	static TInt ChunkSetRestrictions(TInt, TUint);
	static TInt MsgQueueCreate(const TDesC8*, TInt, TInt, TOwnerType);
	static TInt MsgQueueSend(TInt, const TAny*, TInt);
	static TInt MsgQueueReceive(TInt, TAny*, TInt);
	static void MsgQueueNotifySpaceAvailable(TInt, TRequestStatus&);
	static void MsgQueueCancelSpaceAvailable(TInt);
	static void MsgQueueNotifyDataAvailable(TInt, TRequestStatus&);
	static void MsgQueueCancelDataAvailable(TInt);
	static TInt MsgQueueSize(TInt);
	static TInt PropertyDefine(TUint, TUint, TPropertyInfo*);
	static TInt PropertyDelete(TUint, TUint);
	static TInt PropertyAttach(TUint, TUint, TOwnerType);
	static void PropertySubscribe(TInt, TRequestStatus*);
	static void PropertyCancel(TInt);
	static TInt PropertyGetI(TInt, TInt*);
	static TInt PropertyGetB(TInt, TUint8*, TInt);
	static TInt PropertySetI(TInt, TInt);
	static TInt PropertySetB(TInt, const TUint8*, TInt);
	static TInt PropertyFindGetI(TUint, TUint, TInt*);
	static TInt PropertyFindGetB(TUint, TUint, TUint8*, TInt);
	static TInt PropertyFindSetI(TUint, TUint, TInt);
	static TInt PropertyFindSetB(TUint, TUint, TUint8*, TInt);
	static TInt PowerEnableWakeupEvents(TPowerState);
	static void PowerDisableWakeupEvents();
	static void PowerRequestWakeupEventNotification(TRequestStatus*);
	static void PowerCancelWakeupEventNotification();
	static TInt PowerDown();
	static TInt ProcessSetHandleParameter(TInt, TInt, TInt);
	static TInt ProcessSetDataParameter(TInt, TInt, const TUint8*, TInt);
	static TInt ProcessGetHandleParameter(TInt, TObjectType, TOwnerType);
	static TInt ProcessGetDataParameter(TInt, TUint8*, TInt);
	static TInt ProcessDataParameterLength(TInt);
	static TUint MessageClientProcessFlags(TInt);
	static TInt ThreadStackInfo(TInt, TThreadStackInfo&);
	static RAllocator* ThreadGetHeap(TInt);
	static TInt ThreadAsProcess(TInt, TInt);
	static TInt CondVarCreate(const TDesC8*, TOwnerType);
	static TInt CondVarWait(TInt, TInt, TInt);
	static void CondVarSignal(TInt);
	static void CondVarBroadcast(TInt);
	static TInt PlatSecDiagnostic(TPlatSecDiagnostic*);
	static TLinAddr ExceptionDescriptor(TLinAddr);
	static void ThreadRequestSignal(TInt);
	static TBool MutexIsHeld(TInt);
	static TTrapHandler* LeaveStart();
	static void LeaveEnd();
	static void SetDebugMaskIndex(TUint32, TUint);
	static TInt GetModuleNameFromAddress(TAny*, TDes8&);
	static void NotifyChanges(TUint);
	static TInt SetGlobalUserData(TInt, TInt);
	static TInt SessionSecurityInfo(TInt, SSecurityInfo&);
	static const TRequestStatus* MessageClientStatus(TInt);
	static TInt SetFloatingPointMode(TFloatingPointMode, TFloatingPointRoundingMode);
	static TBool BTraceOut(TUint32, TUint32, const BTrace::SExecExtension&, TInt);
	static TBool BTraceOutBig(TUint32, TUint32, const BTrace::SExecExtension&, TInt);
	static TAny* ProcessExeExportData();
	IMPORT_C static TInt SetWin32RuntimeHook(TAny*);
	static TInt GetBTraceId(TInt);
	static void NotifyOnIdle(TRequestStatus*);
	static void CancelMiscNotifier(TRequestStatus*);
	static void NotifyObjectDestruction(TInt, TRequestStatus*);
	static void RegisterTrustedChunk(TInt);
	static TBool UserThreadExiting(TInt);
	};

#ifdef __GEN_USER_EXEC_CODE__
__EXECDECL__ void Exec::WaitForAnyRequest()
	{
	FAST_EXEC0(EFastExecWaitForAnyRequest);
	}

__EXECDECL__ RAllocator* Exec::Heap()
	{
	FAST_EXEC0(EFastExecHeap);
	}

__EXECDECL__ RAllocator* Exec::HeapSwitch(RAllocator*)
	{
	FAST_EXEC1(EFastExecHeapSwitch);
	}

__EXECDECL__ TTrapHandler* Exec::PushTrapFrame(TTrap*)
	{
	FAST_EXEC1(EFastExecPushTrapFrame);
	}

__EXECDECL__ TTrap* Exec::PopTrapFrame()
	{
	FAST_EXEC0(EFastExecPopTrapFrame);
	}

__EXECDECL__ CActiveScheduler* Exec::ActiveScheduler()
	{
	FAST_EXEC0(EFastExecActiveScheduler);
	}

__EXECDECL__ void Exec::SetActiveScheduler(CActiveScheduler*)
	{
	FAST_EXEC1(EFastExecSetActiveScheduler);
	}

__EXECDECL__ TTimerLockSpec Exec::LockPeriod()
	{
	FAST_EXEC0(EFastExecLockPeriod);
	}

__EXECDECL__ TTrapHandler* Exec::TrapHandler()
	{
	FAST_EXEC0(EFastExecTrapHandler);
	}

__EXECDECL__ TTrapHandler* Exec::SetTrapHandler(TTrapHandler*)
	{
	FAST_EXEC1(EFastExecSetTrapHandler);
	}

__EXECDECL__ TInt Exec::LockedInc(TInt&)
	{
	FAST_EXEC1(EFastExecLockedInc);
	}

__EXECDECL__ TInt Exec::LockedDec(TInt&)
	{
	FAST_EXEC1(EFastExecLockedDec);
	}

__EXECDECL__ TUint32 Exec::DebugMask()
	{
	FAST_EXEC0(EFastExecDebugMask);
	}

__EXECDECL__ TUint32 Exec::DebugMaskIndex(TUint)
	{
	FAST_EXEC1(EFastExecDebugMaskIndex);
	}

__EXECDECL__ void Exec::SetDebugMask(TUint32)
	{
	FAST_EXEC1(EFastExecSetDebugMask);
	}

__EXECDECL__ TUint32 Exec::FastCounter()
	{
	FAST_EXEC0(EFastExecFastCounter);
	}

__EXECDECL__ TUint32 Exec::NTickCount()
	{
	FAST_EXEC0(EFastExecNTickCount);
	}

EXPORT_C __EXECDECL__ void UserSvr::LockRamDrive()
	{
	FAST_EXEC0(EFastExecLockRamDrive);
	}

EXPORT_C __EXECDECL__ void UserSvr::UnlockRamDrive()
	{
	FAST_EXEC0(EFastExecUnlockRamDrive);
	}

EXPORT_C __EXECDECL__ TLinAddr UserSvr::RomHeaderAddress()
	{
	FAST_EXEC0(EFastExecRomHeaderAddress);
	}

EXPORT_C __EXECDECL__ TLinAddr UserSvr::RomRootDirectoryAddress()
	{
	FAST_EXEC0(EFastExecRomRootDirectoryAddress);
	}

__EXECDECL__ TInt Exec::SafeInc(TInt&)
	{
	FAST_EXEC1(EFastExecSafeInc);
	}

__EXECDECL__ TInt Exec::SafeDec(TInt&)
	{
	FAST_EXEC1(EFastExecSafeDec);
	}

__EXECDECL__ void Exec::SetReentryPoint(TLinAddr)
	{
	FAST_EXEC1(EFastExecSetReentryPoint);
	}

__EXECDECL__ TUint32 Exec::KernelConfigFlags()
	{
	FAST_EXEC0(EFastExecKernelConfigFlags);
	}

__EXECDECL__ TInt Exec::UTCOffset()
	{
	FAST_EXEC0(EFastExecUTCOffset);
	}

__EXECDECL__ TInt Exec::GetGlobalUserData(TInt)
	{
	FAST_EXEC1(EFastExecGetGlobalUserData);
	}

EXPORT_C __EXECDECL__ TBool BTrace::CheckFilter(TUint32)
	{
	FAST_EXEC1(EFastExecCheckFilter);
	}

__EXECDECL__ TInt Exec::ObjectNext(TObjectType, TBuf8<KMaxFullName>&, TFindHandle&)
	{
	SLOW_EXEC3(EExecObjectNext);
	}

__EXECDECL__ TUint8* Exec::ChunkBase(TInt)
	{
	SLOW_EXEC1(EExecChunkBase);
	}

__EXECDECL__ TInt Exec::ChunkSize(TInt)
	{
	SLOW_EXEC1(EExecChunkSize);
	}

__EXECDECL__ TInt Exec::ChunkMaxSize(TInt)
	{
	SLOW_EXEC1(EExecChunkMaxSize);
	}

__EXECDECL__ TUint Exec::HandleAttributes(TInt)
	{
	SLOW_EXEC1(EExecHandleAttributes);
	}

__EXECDECL__ TUint Exec::TickCount()
	{
	SLOW_EXEC0(EExecTickCount);
	}

__EXECDECL__ void Exec::LogicalDeviceGetCaps(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecLogicalDeviceGetCaps);
	}

__EXECDECL__ TBool Exec::LogicalDeviceQueryVersionSupported(TInt, const TVersion&)
	{
	SLOW_EXEC2(EExecLogicalDeviceQueryVersionSupported);
	}

__EXECDECL__ TBool Exec::LogicalDeviceIsAvailable(TInt, TInt, const TDesC8*, const TDesC8*)
	{
	SLOW_EXEC4(EExecLogicalDeviceIsAvailable);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::LocaleExports(TAny*, TLibraryFunction*)
	{
	SLOW_EXEC2(EExecLocaleExports);
	}

__EXECDECL__ TInt Exec::ChannelRequest(TInt, TInt, TAny*, TAny*)
	{
	SLOW_EXEC4(EExecChannelRequest);
	}

__EXECDECL__ TUint32 Exec::MathRandom()
	{
	SLOW_EXEC0(EExecMathRandom);
	}

__EXECDECL__ void Exec::IMB_Range(TAny*, TUint)
	{
	SLOW_EXEC2(EExecIMBRange);
	}

__EXECDECL__ TInt Exec::ResetMachine(TMachineStartupType)
	{
	SLOW_EXEC1(EExecResetMachine);
	}

__EXECDECL__ TLibraryFunction Exec::LibraryLookup(TInt, TInt)
	{
	SLOW_EXEC2(EExecLibraryLookup);
	}

__EXECDECL__ void Exec::LibraryFileName(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecLibraryFileName);
	}

EXPORT_C __EXECDECL__ TInt UserSvr::ExecuteInSupervisorMode(TSupervisorFunction, TAny*)
	{
	SLOW_EXEC2(EExecExecuteInSupervisorMode);
	}

__EXECDECL__ void Exec::MutexWait(TInt)
	{
	SLOW_EXEC1(EExecMutexWait);
	}

__EXECDECL__ void Exec::MutexSignal(TInt)
	{
	SLOW_EXEC1(EExecMutexSignal);
	}

__EXECDECL__ TInt Exec::ProcessId(TInt)
	{
	SLOW_EXEC1(EExecProcessId);
	}

__EXECDECL__ void Exec::DllFileName(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecDllFileName);
	}

__EXECDECL__ void Exec::ProcessResume(TInt)
	{
	SLOW_EXEC1(EExecProcessResume);
	}

__EXECDECL__ void Exec::ProcessFileName(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecProcessFileName);
	}

__EXECDECL__ void Exec::ProcessCommandLine(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecProcessCommandLine);
	}

__EXECDECL__ TExitType Exec::ProcessExitType(TInt)
	{
	SLOW_EXEC1(EExecProcessExitType);
	}

__EXECDECL__ TInt Exec::ProcessExitReason(TInt)
	{
	SLOW_EXEC1(EExecProcessExitReason);
	}

__EXECDECL__ void Exec::ProcessExitCategory(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecProcessExitCategory);
	}

__EXECDECL__ TProcessPriority Exec::ProcessPriority(TInt)
	{
	SLOW_EXEC1(EExecProcessPriority);
	}

__EXECDECL__ void Exec::ProcessSetPriority(TInt, TProcessPriority)
	{
	SLOW_EXEC2(EExecProcessSetPriority);
	}

__EXECDECL__ TUint Exec::ProcessFlags(TInt)
	{
	SLOW_EXEC1(EExecProcessFlags);
	}

__EXECDECL__ void Exec::ProcessSetFlags(TInt, TUint, TUint)
	{
	SLOW_EXEC3(EExecProcessSetFlags);
	}

__EXECDECL__ TInt Exec::SemaphoreWait(TInt, TInt)
	{
	SLOW_EXEC2(EExecSemaphoreWait);
	}

__EXECDECL__ void Exec::SemaphoreSignal1(TInt)
	{
	SLOW_EXEC1(EExecSemaphoreSignal1);
	}

__EXECDECL__ void Exec::SemaphoreSignalN(TInt, TInt)
	{
	SLOW_EXEC2(EExecSemaphoreSignalN);
	}

__EXECDECL__ void Exec::ServerReceive(TInt, TRequestStatus&, TAny*)
	{
	SLOW_EXEC3(EExecServerReceive);
	}

__EXECDECL__ void Exec::ServerCancel(TInt)
	{
	SLOW_EXEC1(EExecServerCancel);
	}

__EXECDECL__ void Exec::SetSessionPtr(TInt, const TAny*)
	{
	SLOW_EXEC2(EExecSetSessionPtr);
	}

__EXECDECL__ TInt Exec::ThreadId(TInt)
	{
	SLOW_EXEC1(EExecThreadId);
	}

__EXECDECL__ TInt Exec::SessionShare(TInt&, TInt)
	{
	SLOW_EXEC2(EExecSessionShare);
	}

__EXECDECL__ void Exec::ThreadResume(TInt)
	{
	SLOW_EXEC1(EExecThreadResume);
	}

__EXECDECL__ void Exec::ThreadSuspend(TInt)
	{
	SLOW_EXEC1(EExecThreadSuspend);
	}

__EXECDECL__ TThreadPriority Exec::ThreadPriority(TInt)
	{
	SLOW_EXEC1(EExecThreadPriority);
	}

__EXECDECL__ void Exec::ThreadSetPriority(TInt, TThreadPriority)
	{
	SLOW_EXEC2(EExecThreadSetPriority);
	}

__EXECDECL__ TProcessPriority Exec::ThreadProcessPriority(TInt)
	{
	SLOW_EXEC1(EExecThreadProcessPriority);
	}

__EXECDECL__ void Exec::ThreadSetProcessPriority(TInt, TProcessPriority)
	{
	SLOW_EXEC2(EExecThreadSetProcessPriority);
	}

__EXECDECL__ TUint Exec::ThreadFlags(TInt)
	{
	SLOW_EXEC1(EExecThreadFlags);
	}

__EXECDECL__ void Exec::ThreadSetFlags(TInt, TUint, TUint)
	{
	SLOW_EXEC3(EExecThreadSetFlags);
	}

__EXECDECL__ TInt Exec::ThreadRequestCount(TInt)
	{
	SLOW_EXEC1(EExecThreadRequestCount);
	}

__EXECDECL__ TExitType Exec::ThreadExitType(TInt)
	{
	SLOW_EXEC1(EExecThreadExitType);
	}

__EXECDECL__ TInt Exec::ThreadExitReason(TInt)
	{
	SLOW_EXEC1(EExecThreadExitReason);
	}

__EXECDECL__ void Exec::ThreadExitCategory(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecThreadExitCategory);
	}

__EXECDECL__ void Exec::TimerCancel(TInt)
	{
	SLOW_EXEC1(EExecTimerCancel);
	}

__EXECDECL__ void Exec::TimerAfter(TInt, TRequestStatus&, TInt)
	{
	SLOW_EXEC3(EExecTimerAfter);
	}

__EXECDECL__ void Exec::TimerAt(TInt, TRequestStatus&, const EXEC_TIME&)
	{
	SLOW_EXEC3(EExecTimerAt);
	}

__EXECDECL__ void Exec::TimerLock(TInt, TRequestStatus&, TTimerLockSpec)
	{
	SLOW_EXEC3(EExecTimerLock);
	}

__EXECDECL__ TInt Exec::ChangeNotifierLogon(TInt, TRequestStatus&)
	{
	SLOW_EXEC2(EExecChangeNotifierLogon);
	}

__EXECDECL__ TInt Exec::ChangeNotifierLogoff(TInt)
	{
	SLOW_EXEC1(EExecChangeNotifierLogoff);
	}

__EXECDECL__ void Exec::RequestSignal(TInt)
	{
	SLOW_EXEC1(EExecRequestSignal);
	}

__EXECDECL__ void Exec::HandleName(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecHandleName);
	}

__EXECDECL__ void Exec::HandleFullName(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecHandleFullName);
	}

__EXECDECL__ void Exec::HandleInfo(TInt, THandleInfo*)
	{
	SLOW_EXEC2(EExecHandleInfo);
	}

__EXECDECL__ void Exec::HandleCount(TInt, TInt&, TInt&)
	{
	SLOW_EXEC3(EExecHandleCount);
	}

__EXECDECL__ void Exec::After(TInt, TRequestStatus&)
	{
	SLOW_EXEC2(EExecAfter);
	}

__EXECDECL__ void Exec::At(const EXEC_TIME&, TRequestStatus&)
	{
	SLOW_EXEC2(EExecAt);
	}

__EXECDECL__ void Exec::MessageComplete(TInt, TInt)
	{
	SLOW_EXEC2(EExecMessageComplete);
	}

__EXECDECL__ void Exec::MessageCompleteWithHandle(TInt, TInt)
	{
	SLOW_EXEC2(EExecMessageCompleteWithHandle);
	}

__EXECDECL__ TInt Exec::TimeNow(EXEC_TIME&, TInt&)
	{
	SLOW_EXEC2(EExecTimeNow);
	}

__EXECDECL__ TInt Exec::TimeNowSecure(EXEC_TIME&, TInt&)
	{
	SLOW_EXEC2(EExecTimeNowSecure);
	}

__EXECDECL__ TInt Exec::SetUTCTimeAndOffset(const EXEC_TIME&, TInt, TUint, TUint)
	{
	SLOW_EXEC4(EExecSetUTCTimeAndOffset);
	}

__EXECDECL__ TInt Exec::SetMachineConfiguration(const TDesC8&)
	{
	SLOW_EXEC1(EExecSetMachineConfiguration);
	}

__EXECDECL__ void Exec::CaptureEventHook()
	{
	SLOW_EXEC0(EExecCaptureEventHook);
	}

__EXECDECL__ void Exec::ReleaseEventHook()
	{
	SLOW_EXEC0(EExecReleaseEventHook);
	}

__EXECDECL__ void Exec::RequestEvent(TRawEventBuf&, TRequestStatus&)
	{
	SLOW_EXEC2(EExecRequestEvent);
	}

__EXECDECL__ void Exec::RequestEventCancel()
	{
	SLOW_EXEC0(EExecRequestEventCancel);
	}

__EXECDECL__ TInt Exec::AddEvent(const TRawEvent&)
	{
	SLOW_EXEC1(EExecAddEvent);
	}

__EXECDECL__ TAny* Exec::DllTls(TInt, TInt)
	{
	SLOW_EXEC2(EExecDllTls);
	}

__EXECDECL__ TInt Exec::HalFunction(TInt, TInt, TAny*, TAny*)
	{
	SLOW_EXEC4(EExecHalFunction);
	}

__EXECDECL__ void Exec::WsRegisterThread()
	{
	SLOW_EXEC0(EExecWsRegisterThread);
	}

__EXECDECL__ void Exec::FsRegisterThread()
	{
	SLOW_EXEC0(EExecFsRegisterThread);
	}

__EXECDECL__ TInt Exec::ProcessCommandLineLength(TInt)
	{
	SLOW_EXEC1(EExecProcessCommandLineLength);
	}

__EXECDECL__ void Exec::TimerInactivity(TInt, TRequestStatus&, TInt)
	{
	SLOW_EXEC3(EExecTimerInactivity);
	}

__EXECDECL__ TInt Exec::UserInactivityTime()
	{
	SLOW_EXEC0(EExecUserInactivityTime);
	}

__EXECDECL__ void Exec::ResetInactivityTime()
	{
	SLOW_EXEC0(EExecResetInactivityTime);
	}

__EXECDECL__ void Exec::DebugPrint(TAny*, TInt)
	{
	SLOW_EXEC2(EExecDebugPrint);
	}

__EXECDECL__ TInt Exec::BreakPoint()
	{
	SLOW_EXEC0(EExecBreakPoint);
	}

__EXECDECL__ TInt Exec::ProfileStart(TInt)
	{
	SLOW_EXEC1(EExecProfileStart);
	}

__EXECDECL__ TInt Exec::ProfileEnd(TInt)
	{
	SLOW_EXEC1(EExecProfileEnd);
	}

__EXECDECL__ TExceptionHandler Exec::ExceptionHandler(TInt)
	{
	SLOW_EXEC1(EExecExceptionHandler);
	}

__EXECDECL__ TInt Exec::SetExceptionHandler(TInt, TExceptionHandler, TUint32)
	{
	SLOW_EXEC3(EExecSetExceptionHandler);
	}

__EXECDECL__ void Exec::ModifyExceptionMask(TInt, TUint32, TUint32)
	{
	SLOW_EXEC3(EExecModifyExceptionMask);
	}

__EXECDECL__ TInt Exec::RaiseException(TInt, TExcType)
	{
	SLOW_EXEC2(EExecRaiseException);
	}

__EXECDECL__ TInt Exec::IsExceptionHandled(TInt, TExcType, TBool)
	{
	SLOW_EXEC3(EExecIsExceptionHandled);
	}

__EXECDECL__ TInt Exec::ProcessGetMemoryInfo(TInt, TModuleMemoryInfo&)
	{
	SLOW_EXEC2(EExecProcessGetMemoryInfo);
	}

__EXECDECL__ TInt Exec::LibraryGetMemoryInfo(TInt, TModuleMemoryInfo&)
	{
	SLOW_EXEC2(EExecLibraryGetMemoryInfo);
	}

__EXECDECL__ TInt Exec::MachineConfiguration(TDes8&, TInt&)
	{
	SLOW_EXEC2(EExecMachineConfiguration);
	}

__EXECDECL__ TInt Exec::SetMemoryThresholds(TInt, TInt)
	{
	SLOW_EXEC2(EExecSetMemoryThresholds);
	}

__EXECDECL__ void Exec::LibraryType(TInt, TUidType&)
	{
	SLOW_EXEC2(EExecLibraryType);
	}

__EXECDECL__ void Exec::ProcessType(TInt, TUidType&)
	{
	SLOW_EXEC2(EExecProcessType);
	}

__EXECDECL__ TInt Exec::ChunkBottom(TInt)
	{
	SLOW_EXEC1(EExecChunkBottom);
	}

__EXECDECL__ TInt Exec::ChunkTop(TInt)
	{
	SLOW_EXEC1(EExecChunkTop);
	}

__EXECDECL__ void Exec::ThreadContext(TInt, TDes8&)
	{
	SLOW_EXEC2(EExecThreadContext);
	}

__EXECDECL__ TInt Exec::ThreadCreate(const TDesC8&, TOwnerType, SThreadCreateInfo8&)
	{
	SLOW_EXEC3(EExecThreadCreate);
	}

__EXECDECL__ TInt Exec::FindHandleOpen(TOwnerType, const TFindHandle&)
	{
	SLOW_EXEC2(EExecFindHandleOpen);
	}

__EXECDECL__ TInt Exec::HandleClose(TInt)
	{
	SLOW_EXEC1(EExecHandleClose);
	}

__EXECDECL__ TInt Exec::ChunkCreate(TOwnerType, const TDesC8*, TChunkCreate&)
	{
	SLOW_EXEC3(EExecChunkCreate);
	}

__EXECDECL__ TInt Exec::ChunkAdjust(TInt, TInt, TInt, TInt)
	{
	SLOW_EXEC4(EExecChunkAdjust);
	}

__EXECDECL__ TInt Exec::OpenObject(TObjectType, const TDesC8&, TOwnerType)
	{
	SLOW_EXEC3(EExecOpenObject);
	}

__EXECDECL__ TInt Exec::HandleDuplicate(TInt, TOwnerType, TInt)
	{
	SLOW_EXEC3(EExecHandleDuplicate);
	}

__EXECDECL__ TInt Exec::MutexCreate(const TDesC8*, TOwnerType)
	{
	SLOW_EXEC2(EExecMutexCreate);
	}

__EXECDECL__ TInt Exec::SemaphoreCreate(const TDesC8*, TInt, TOwnerType)
	{
	SLOW_EXEC3(EExecSemaphoreCreate);
	}

__EXECDECL__ TInt Exec::ThreadOpenById(TUint, TOwnerType)
	{
	SLOW_EXEC2(EExecThreadOpenById);
	}

__EXECDECL__ TInt Exec::ProcessOpenById(TUint, TOwnerType)
	{
	SLOW_EXEC2(EExecProcessOpenById);
	}

__EXECDECL__ void Exec::ThreadKill(TInt, TExitType, TInt, const TDesC8*)
	{
	SLOW_EXEC4(EExecThreadKill);
	}

__EXECDECL__ void Exec::ThreadLogon(TInt, TRequestStatus*, TBool)
	{
	SLOW_EXEC3(EExecThreadLogon);
	}

__EXECDECL__ TInt Exec::ThreadLogonCancel(TInt, TRequestStatus*, TBool)
	{
	SLOW_EXEC3(EExecThreadLogonCancel);
	}

__EXECDECL__ TInt Exec::DllSetTls(TInt, TInt, TAny*)
	{
	SLOW_EXEC3(EExecDllSetTls);
	}

__EXECDECL__ void Exec::DllFreeTls(TInt)
	{
	SLOW_EXEC1(EExecDllFreeTls);
	}

__EXECDECL__ TInt Exec::ThreadRename(TInt, const TDesC8&)
	{
	SLOW_EXEC2(EExecThreadRename);
	}

__EXECDECL__ TInt Exec::ProcessRename(TInt, const TDesC8&)
	{
	SLOW_EXEC2(EExecProcessRename);
	}

__EXECDECL__ void Exec::ProcessKill(TInt, TExitType, TInt, const TDesC8*)
	{
	SLOW_EXEC4(EExecProcessKill);
	}

__EXECDECL__ void Exec::ProcessLogon(TInt, TRequestStatus*, TBool)
	{
	SLOW_EXEC3(EExecProcessLogon);
	}

__EXECDECL__ TInt Exec::ProcessLogonCancel(TInt, TRequestStatus*, TBool)
	{
	SLOW_EXEC3(EExecProcessLogonCancel);
	}

__EXECDECL__ TInt Exec::ThreadProcess(TInt)
	{
	SLOW_EXEC1(EExecThreadProcess);
	}

__EXECDECL__ TInt Exec::ServerCreate(const TDesC8*, TInt)
	{
	SLOW_EXEC2(EExecServerCreate);
	}

__EXECDECL__ TInt Exec::SessionCreate(const TDesC8&, TInt, const TSecurityPolicy*, TInt)
	{
	SLOW_EXEC4(EExecSessionCreate);
	}

__EXECDECL__ TInt Exec::SessionCreateFromHandle(TInt, TInt, const TSecurityPolicy*, TInt)
	{
	SLOW_EXEC4(EExecSessionCreateFromHandle);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::DeviceLoad(TAny*, TInt)
	{
	SLOW_EXEC2(EExecDeviceLoad);
	}

__EXECDECL__ TInt Exec::DeviceFree(const TDesC8&, TInt)
	{
	SLOW_EXEC2(EExecDeviceFree);
	}

__EXECDECL__ TInt Exec::ChannelCreate(const TDesC8&, TChannelCreateInfo8&, TInt)
	{
	SLOW_EXEC3(EExecChannelCreate);
	}

__EXECDECL__ TInt Exec::TimerCreate()
	{
	SLOW_EXEC0(EExecTimerCreate);
	}

__EXECDECL__ void Exec::TimerHighRes(TInt, TRequestStatus&, TInt)
	{
	SLOW_EXEC3(EExecTimerHighRes);
	}

__EXECDECL__ void Exec::AfterHighRes(TInt, TRequestStatus&)
	{
	SLOW_EXEC2(EExecAfterHighRes);
	}

__EXECDECL__ TInt Exec::ChangeNotifierCreate(TOwnerType)
	{
	SLOW_EXEC1(EExecChangeNotifierCreate);
	}

__EXECDECL__ TInt Exec::UndertakerCreate(TOwnerType)
	{
	SLOW_EXEC1(EExecUndertakerCreate);
	}

__EXECDECL__ TInt Exec::UndertakerLogon(TInt, TRequestStatus&, TInt&)
	{
	SLOW_EXEC3(EExecUndertakerLogon);
	}

__EXECDECL__ TInt Exec::UndertakerLogonCancel(TInt)
	{
	SLOW_EXEC1(EExecUndertakerLogonCancel);
	}

__EXECDECL__ void Exec::KernelHeapDebug(TInt, TInt, TAny*)
	{
	SLOW_EXEC3(EExecKernelHeapDebug);
	}

__EXECDECL__ TInt Exec::ThreadGetCpuTime(TInt, EXEC_INT64&)
	{
	SLOW_EXEC2(EExecThreadGetCpuTime);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::CodeSegCreate(TCodeSegCreateInfo&)
	{
	SLOW_EXEC1(EExecCodeSegCreate);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::CodeSegLoaded(TCodeSegCreateInfo&)
	{
	SLOW_EXEC1(EExecCodeSegLoaded);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::LibraryCreate(TLibraryCreateInfo&)
	{
	SLOW_EXEC1(EExecLibraryCreate);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::CodeSegOpen(TAny*, TInt)
	{
	SLOW_EXEC2(EExecCodeSegOpen);
	}

EXPORT_C __EXECDECL__ void E32Loader::CodeSegClose(TAny*)
	{
	SLOW_EXEC1(EExecCodeSegClose);
	}

EXPORT_C __EXECDECL__ void E32Loader::CodeSegNext(TAny*&, const TFindCodeSeg&)
	{
	SLOW_EXEC2(EExecCodeSegNext);
	}

EXPORT_C __EXECDECL__ void E32Loader::CodeSegInfo(TAny*, TCodeSegCreateInfo&)
	{
	SLOW_EXEC2(EExecCodeSegInfo);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::CodeSegAddDependency(TAny*, TAny*)
	{
	SLOW_EXEC2(EExecCodeSegAddDependency);
	}

EXPORT_C __EXECDECL__ void E32Loader::CodeSegDeferDeletes()
	{
	SLOW_EXEC0(EExecCodeSegDeferDeletes);
	}

EXPORT_C __EXECDECL__ void E32Loader::CodeSegEndDeferDeletes()
	{
	SLOW_EXEC0(EExecCodeSegEndDeferDeletes);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::ProcessCreate(TProcessCreateInfo&, const TDesC8*)
	{
	SLOW_EXEC2(EExecProcessCreate);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::ProcessLoaded(TProcessCreateInfo&)
	{
	SLOW_EXEC1(EExecProcessLoaded);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::CheckClientState(TInt)
	{
	SLOW_EXEC1(EExecCheckLoaderClientState);
	}

EXPORT_C __EXECDECL__ TAny* E32Loader::ThreadProcessCodeSeg(TInt)
	{
	SLOW_EXEC1(EExecThreadProcessCodeSeg);
	}

EXPORT_C __EXECDECL__ void E32Loader::ReadExportDir(TAny*, TLinAddr*)
	{
	SLOW_EXEC2(EExecCodeSegReadExportDir);
	}

__EXECDECL__ TInt E32Loader::WaitDllLock()
	{
	SLOW_EXEC0(EExecWaitDllLock);
	}

__EXECDECL__ TInt E32Loader::ReleaseDllLock()
	{
	SLOW_EXEC0(EExecReleaseDllLock);
	}

__EXECDECL__ TInt E32Loader::LibraryAttach(TInt, TInt&, TLinAddr*)
	{
	SLOW_EXEC3(EExecLibraryAttach);
	}

__EXECDECL__ TInt E32Loader::LibraryAttached(TInt)
	{
	SLOW_EXEC1(EExecLibraryAttached);
	}

__EXECDECL__ TInt E32Loader::StaticCallList(TInt&, TLinAddr*)
	{
	SLOW_EXEC2(EExecStaticCallList);
	}

__EXECDECL__ TInt E32Loader::LibraryDetach(TInt&, TLinAddr*)
	{
	SLOW_EXEC2(EExecLibraryDetach);
	}

__EXECDECL__ TInt E32Loader::LibraryDetached()
	{
	SLOW_EXEC0(EExecLibraryDetached);
	}

__EXECDECL__ TInt Exec::LastThreadHandle()
	{
	SLOW_EXEC0(EExecLastThreadHandle);
	}

__EXECDECL__ void Exec::ThreadRendezvous(TInt)
	{
	SLOW_EXEC1(EExecThreadRendezvous);
	}

__EXECDECL__ void Exec::ProcessRendezvous(TInt)
	{
	SLOW_EXEC1(EExecProcessRendezvous);
	}

__EXECDECL__ TInt Exec::MessageGetDesLength(TInt, TInt)
	{
	SLOW_EXEC2(EExecMessageGetDesLength);
	}

__EXECDECL__ TInt Exec::MessageGetDesMaxLength(TInt, TInt)
	{
	SLOW_EXEC2(EExecMessageGetDesMaxLength);
	}

__EXECDECL__ TInt Exec::MessageClient(TInt, TOwnerType)
	{
	SLOW_EXEC2(EExecMessageClient);
	}

__EXECDECL__ TInt Exec::MessageSetProcessPriority(TInt, TProcessPriority)
	{
	SLOW_EXEC2(EExecMessageSetProcessPriority);
	}

__EXECDECL__ void Exec::MessageConstructFromPtr(TInt, TAny*)
	{
	SLOW_EXEC2(EExecMessageConstructFromPtr);
	}

__EXECDECL__ void Exec::MessageKill(TInt, TExitType, TInt, const TDesC8*)
	{
	SLOW_EXEC4(EExecMessageKill);
	}

__EXECDECL__ TInt Exec::MessageOpenObject(TInt, TObjectType, TInt, TOwnerType)
	{
	SLOW_EXEC4(EExecMessageOpenObject);
	}

__EXECDECL__ void Exec::ProcessSecurityInfo(TInt, SSecurityInfo&)
	{
	SLOW_EXEC2(EExecProcessSecurityInfo);
	}

__EXECDECL__ void Exec::ThreadSecurityInfo(TInt, SSecurityInfo&)
	{
	SLOW_EXEC2(EExecThreadSecurityInfo);
	}

__EXECDECL__ void Exec::MessageSecurityInfo(TInt, SSecurityInfo&)
	{
	SLOW_EXEC2(EExecMessageSecurityInfo);
	}

__EXECDECL__ void Exec::CreatorSecurityInfo(SSecurityInfo&)
	{
	SLOW_EXEC1(EExecCreatorSecurityInfo);
	}

__EXECDECL__ void Exec::DisabledCapabilities(SCapabilitySet&)
	{
	SLOW_EXEC1(EExecDisabledCapabilities);
	}

__EXECDECL__ TInt Exec::ChunkSetRestrictions(TInt, TUint)
	{
	SLOW_EXEC2(EExecChunkSetRestrictions);
	}

__EXECDECL__ TInt Exec::MsgQueueCreate(const TDesC8*, TInt, TInt, TOwnerType)
	{
	SLOW_EXEC4(EExecMsgQueueCreate);
	}

__EXECDECL__ TInt Exec::MsgQueueSend(TInt, const TAny*, TInt)
	{
	SLOW_EXEC3(EExecMsgQueueSend);
	}

__EXECDECL__ TInt Exec::MsgQueueReceive(TInt, TAny*, TInt)
	{
	SLOW_EXEC3(EExecMsgQueueReceive);
	}

__EXECDECL__ void Exec::MsgQueueNotifySpaceAvailable(TInt, TRequestStatus&)
	{
	SLOW_EXEC2(EExecMsgQueueNotifySpaceAvailable);
	}

__EXECDECL__ void Exec::MsgQueueCancelSpaceAvailable(TInt)
	{
	SLOW_EXEC1(EExecMsgQueueCancelSpaceAvailable);
	}

__EXECDECL__ void Exec::MsgQueueNotifyDataAvailable(TInt, TRequestStatus&)
	{
	SLOW_EXEC2(EExecMsgQueueNotifyDataAvailable);
	}

__EXECDECL__ void Exec::MsgQueueCancelDataAvailable(TInt)
	{
	SLOW_EXEC1(EExecMsgQueueCancelDataAvailable);
	}

__EXECDECL__ TInt Exec::MsgQueueSize(TInt)
	{
	SLOW_EXEC1(EExecMsgQueueSize);
	}

__EXECDECL__ TInt Exec::PropertyDefine(TUint, TUint, TPropertyInfo*)
	{
	SLOW_EXEC3(EExecPropertyDefine);
	}

__EXECDECL__ TInt Exec::PropertyDelete(TUint, TUint)
	{
	SLOW_EXEC2(EExecPropertyDelete);
	}

__EXECDECL__ TInt Exec::PropertyAttach(TUint, TUint, TOwnerType)
	{
	SLOW_EXEC3(EExecPropertyAttach);
	}

__EXECDECL__ void Exec::PropertySubscribe(TInt, TRequestStatus*)
	{
	SLOW_EXEC2(EExecPropertySubscribe);
	}

__EXECDECL__ void Exec::PropertyCancel(TInt)
	{
	SLOW_EXEC1(EExecPropertyCancel);
	}

__EXECDECL__ TInt Exec::PropertyGetI(TInt, TInt*)
	{
	SLOW_EXEC2(EExecPropertyGetI);
	}

__EXECDECL__ TInt Exec::PropertyGetB(TInt, TUint8*, TInt)
	{
	SLOW_EXEC3(EExecPropertyGetB);
	}

__EXECDECL__ TInt Exec::PropertySetI(TInt, TInt)
	{
	SLOW_EXEC2(EExecPropertySetI);
	}

__EXECDECL__ TInt Exec::PropertySetB(TInt, const TUint8*, TInt)
	{
	SLOW_EXEC3(EExecPropertySetB);
	}

__EXECDECL__ TInt Exec::PropertyFindGetI(TUint, TUint, TInt*)
	{
	SLOW_EXEC3(EExecPropertyFindGetI);
	}

__EXECDECL__ TInt Exec::PropertyFindGetB(TUint, TUint, TUint8*, TInt)
	{
	SLOW_EXEC4(EExecPropertyFindGetB);
	}

__EXECDECL__ TInt Exec::PropertyFindSetI(TUint, TUint, TInt)
	{
	SLOW_EXEC3(EExecPropertyFindSetI);
	}

__EXECDECL__ TInt Exec::PropertyFindSetB(TUint, TUint, TUint8*, TInt)
	{
	SLOW_EXEC4(EExecPropertyFindSetB);
	}

__EXECDECL__ TInt Exec::PowerEnableWakeupEvents(TPowerState)
	{
	SLOW_EXEC1(EExecPowerEnableWakeupEvents);
	}

__EXECDECL__ void Exec::PowerDisableWakeupEvents()
	{
	SLOW_EXEC0(EExecPowerDisableWakeupEvents);
	}

__EXECDECL__ void Exec::PowerRequestWakeupEventNotification(TRequestStatus*)
	{
	SLOW_EXEC1(EExecPowerRequestWakeupEventNotification);
	}

__EXECDECL__ void Exec::PowerCancelWakeupEventNotification()
	{
	SLOW_EXEC0(EExecPowerCancelWakeupEventNotification);
	}

__EXECDECL__ TInt Exec::PowerDown()
	{
	SLOW_EXEC0(EExecPowerDown);
	}

__EXECDECL__ TInt Exec::ProcessSetHandleParameter(TInt, TInt, TInt)
	{
	SLOW_EXEC3(EExecProcessSetHandleParameter);
	}

__EXECDECL__ TInt Exec::ProcessSetDataParameter(TInt, TInt, const TUint8*, TInt)
	{
	SLOW_EXEC4(EExecProcessSetDataParameter);
	}

__EXECDECL__ TInt Exec::ProcessGetHandleParameter(TInt, TObjectType, TOwnerType)
	{
	SLOW_EXEC3(EExecProcessGetHandleParameter);
	}

__EXECDECL__ TInt Exec::ProcessGetDataParameter(TInt, TUint8*, TInt)
	{
	SLOW_EXEC3(EExecProcessGetDataParameter);
	}

__EXECDECL__ TInt Exec::ProcessDataParameterLength(TInt)
	{
	SLOW_EXEC1(EExecProcessDataParameterLength);
	}

__EXECDECL__ TUint Exec::MessageClientProcessFlags(TInt)
	{
	SLOW_EXEC1(EExecMessageClientProcessFlags);
	}

__EXECDECL__ TInt Exec::ThreadStackInfo(TInt, TThreadStackInfo&)
	{
	SLOW_EXEC2(EExecThreadStackInfo);
	}

__EXECDECL__ RAllocator* Exec::ThreadGetHeap(TInt)
	{
	SLOW_EXEC1(EExecThreadGetHeap);
	}

__EXECDECL__ TInt Exec::ThreadAsProcess(TInt, TInt)
	{
	SLOW_EXEC2(EExecThreadAsProcess);
	}

__EXECDECL__ TInt Exec::CondVarCreate(const TDesC8*, TOwnerType)
	{
	SLOW_EXEC2(EExecCondVarCreate);
	}

__EXECDECL__ TInt Exec::CondVarWait(TInt, TInt, TInt)
	{
	SLOW_EXEC3(EExecCondVarWait);
	}

__EXECDECL__ void Exec::CondVarSignal(TInt)
	{
	SLOW_EXEC1(EExecCondVarSignal);
	}

__EXECDECL__ void Exec::CondVarBroadcast(TInt)
	{
	SLOW_EXEC1(EExecCondVarBroadcast);
	}

__EXECDECL__ TInt Exec::PlatSecDiagnostic(TPlatSecDiagnostic*)
	{
	SLOW_EXEC1(EExecPlatSecDiagnostic);
	}

__EXECDECL__ TLinAddr Exec::ExceptionDescriptor(TLinAddr)
	{
	SLOW_EXEC1(EExecExceptionDescriptor);
	}

__EXECDECL__ void Exec::ThreadRequestSignal(TInt)
	{
	SLOW_EXEC1(EExecThreadRequestSignal);
	}

__EXECDECL__ TBool Exec::MutexIsHeld(TInt)
	{
	SLOW_EXEC1(EExecMutexIsHeld);
	}

__EXECDECL__ TTrapHandler* Exec::LeaveStart()
	{
	SLOW_EXEC0(EExecLeaveStart);
	}

__EXECDECL__ void Exec::LeaveEnd()
	{
	SLOW_EXEC0(EExecLeaveEnd);
	}

__EXECDECL__ void Exec::SetDebugMaskIndex(TUint32, TUint)
	{
	SLOW_EXEC2(EExecSetDebugMaskIndex);
	}

__EXECDECL__ TInt Exec::GetModuleNameFromAddress(TAny*, TDes8&)
	{
	SLOW_EXEC2(EExecGetModuleNameFromAddress);
	}

__EXECDECL__ void Exec::NotifyChanges(TUint)
	{
	SLOW_EXEC1(EExecNotifyChanges);
	}

__EXECDECL__ TInt Exec::SetGlobalUserData(TInt, TInt)
	{
	SLOW_EXEC2(EExecSetGlobalUserData);
	}

__EXECDECL__ TInt Exec::SessionSecurityInfo(TInt, SSecurityInfo&)
	{
	SLOW_EXEC2(EExecSessionSecurityInfo);
	}

__EXECDECL__ const TRequestStatus* Exec::MessageClientStatus(TInt)
	{
	SLOW_EXEC1(EExecMessageClientStatus);
	}

__EXECDECL__ TInt Exec::SetFloatingPointMode(TFloatingPointMode, TFloatingPointRoundingMode)
	{
	SLOW_EXEC2(EExecSetFloatingPointMode);
	}

EXPORT_C __EXECDECL__ TBool BTrace::CheckFilter2(TUint32, TUint32)
	{
	SLOW_EXEC2(EExecCheckFilter2);
	}

__EXECDECL__ TAny* Exec::ProcessExeExportData()
	{
	SLOW_EXEC0(EExecProcessExeExportData);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::NotifyIfCodeSegDestroyed(TRequestStatus&)
	{
	SLOW_EXEC1(EExecNotifyIfCodeSegDestroyed);
	}

EXPORT_C __EXECDECL__ TInt E32Loader::GetDestroyedCodeSegInfo(TCodeSegLoaderCookie&)
	{
	SLOW_EXEC1(EExecGetDestroyedCodeSegInfo);
	}

EXPORT_C __EXECDECL__ TInt Exec::SetWin32RuntimeHook(TAny*)
	{
	SLOW_EXEC1(EExecSetWin32RuntimeHook);
	}

__EXECDECL__ TInt Exec::GetBTraceId(TInt)
	{
	SLOW_EXEC1(EExecGetBTraceId);
	}

__EXECDECL__ void Exec::NotifyOnIdle(TRequestStatus*)
	{
	SLOW_EXEC1(EExecNotifyOnIdle);
	}

__EXECDECL__ void Exec::CancelMiscNotifier(TRequestStatus*)
	{
	SLOW_EXEC1(EExecCancelMiscNotifier);
	}

__EXECDECL__ void Exec::NotifyObjectDestruction(TInt, TRequestStatus*)
	{
	SLOW_EXEC2(EExecNotifyObjectDestruction);
	}

__EXECDECL__ void Exec::RegisterTrustedChunk(TInt)
	{
	SLOW_EXEC1(EExecRegisterTrustedChunk);
	}

__EXECDECL__ TBool Exec::UserThreadExiting(TInt)
	{
	SLOW_EXEC1(EExecUserThreadExiting);
	}

#endif
