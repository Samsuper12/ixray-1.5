#include "stdafx.h"

#include "net_light.h"
#include "hxgrid/Interface/IAgent.h"
#include "hxgrid/Interface/IGenericStream.h"
#include "net_light_task.h"
#include "net_stream.h"
#include "xrlc_globaldata.h"
#include "xrdeflector.h"
#include "net_task.h"
//==============================================================================
// function RunTask(): boolean;
//==============================================================================


static xrCriticalSection	block;
LPCSTR dataDesc  = "global_data";

xr_vector<u32>	net_pool;
/*
static struct unload
{
	IAgent* agent;
	~unload()
	{
		if(!agent)
			return;
		LPCRITICAL_SECTION gcs;
		agent->GetGlobalCriticalSection( &gcs );
		LeaveCriticalSection( gcs );
	}
} _unload;
*/
DWORD g_sessionId = DWORD(-1);
class net_task_interface_impl : public net_task_interface
{
 
 bool  GetGlobalData( IAgent* agent,
				    DWORD sessionId )
 {
	 
	 if(!inlc_global_data())
	 {

		  net_pool.clear();
		  IGenericStream* globalDataStream;
		  HRESULT rz = agent->GetData(sessionId, dataDesc, &globalDataStream);
		  
		  if (rz!=S_OK) 
		  {
			  //block.Leave();
			  return false;
		  } 

		 INetReader r_global( globalDataStream );
		 create_global_data();

		 VERIFY( inlc_global_data() );
		 inlc_global_data()->read( r_global );

		 globalDataStream->Release();
		   
		 agent->FreeCachedData(sessionId, dataDesc);
		 FPU::m64r		();
		 Memory.mem_compact	();
	 }
	 return true;

 }

 bool  TaskReceive( net_task &task,IAgent* agent,
					DWORD sessionId, 
					IGenericStream* inStream )
 {
	
	 bool ret = false;
	 __try{
		ret = GetGlobalData( agent, sessionId ) && task.receive( inStream ) ;
	 }
	 __except( EXCEPTION_EXECUTE_HANDLER )
	 {
		return ret;
	 }
	return ret;
 }

 bool TaskSendResult( net_task &task,  IGenericStream* outStream )
 {
	
	bool ret = false;
	__try{
		ret = task.send( outStream ) ;
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		return ret;
	}
	return ret;
}

bool  RunTask ( IAgent* agent,
                 DWORD sessionId,
                 IGenericStream* inStream,
                 IGenericStream* outStream )
{
 
 block.Enter();

 g_sessionId = sessionId;
 net_task task( *agent, sessionId );

 if(!TaskReceive( task, agent, sessionId, inStream ))
 {
	 block.Leave();
	 return false;
 }
 block.Leave();

 task.run	();

 block.Enter();
 
 if(!TaskSendResult( task, outStream ))
 {
	 block.Leave();
	 return false;
 }
 block.Leave();
 return true;
}
} g_net_task_interface_impl;


XRLC_LIGHT_API net_task_interface *g_net_task_interface = &g_net_task_interface_impl;
/*
XRLC_LIGHT_API  void __cdecl  EndSession(IAgent* agent, DWORD sessionId)
{
	LPCRITICAL_SECTION gcs;
	  agent->GetGlobalCriticalSection( &gcs );
	  //LeaveCriticalSection( gcs );


*/