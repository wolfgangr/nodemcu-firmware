/*
 * LUA binding for lwIP netif interface
 * Jan 2017 - Wolfgang Rosner
 *
 * http://git.savannah.gnu.org/cgit/lwip.git/tree/src/include/lwip/netif.h
 * 
 * local ressouces
 * 	./app/include/lwip/netif.h
 * 	./app/include/lwip/netifapi.h
 * 	./app/include/netif/*.h
 * 	./app/lwip/api/netifapi.c
 * 	./app/lwip/core/netif.c
 * 	./app/lwip/netif/etharp.c
 * 
 * while we expect to be able to use the "direct" api
 *	netif API (to be used from TCPIP thread)
 * 	from netif.h
 * 
 * we'll start with API interface
 *  	netif API (to be used from non-TCPIP threads) 
 *      from netifapi.h
 * 
 * this interface 
 * - looks much simpler
 * - does not require callbacks
 *     so we expcet the lwIP OS emulation layer do the proper timer handling...
 * - seems to fit the requirement for basic handling of ethernet drivers...
 *
 *  see Adam Dunkel's book.... 12.2
 *  	The general design principle used is to let as much work as possible be done 
 *	within the application process rather than in the TCP/IP process. 
 *	This is important since all processes use the TCP/IP process for their TCP/IP communication. 
 *
 *
 *
 */



#include "module.h"
#include "lauxlib.h"
#include "platform.h"

#include "c_stdlib.h"
#include "c_stdio.h"

#include "lwip/netifapi.h"
#include "lwip/netif.h"

// other dependencies should be chain loaded, I suppose



static lua_State *gL = NULL;

static int first_IF( lua_State *L) {

  c_printf("netif_list: 0x%X \n", netif_list ) ;
  // return 1;
  return 0;
}


/*
struct netif {
  ** pointer to next in linked list *
  struct netif *next;

  ** IP address configuration in network byte order *
  ip_addr_t ip_addr;
  ip_addr_t netmask;
  ip_addr_t gw;
 */


static int first_adr( lua_State *L) {

  c_printf("first interfaces at 0x%X next: 0x%X  IP: %X netmask: %X gw: %X  \n", 
	netif_list, netif_list->next,  netif_list->ip_addr, netif_list->netmask, netif_list->gw 
   );
  // return 1;
  return 0;
}


void print_adr(struct netif *ifc) {
  c_printf(" IP: %X netmask: %X gw: %X  \n",
        ifc->ip_addr, ifc->netmask, ifc->gw
   );

}


static int first_adr2( lua_State *L) {
  c_printf("first interface print2 - ");
  print_adr(netif_list);
  return 0;
}



/*
   *  to pass a packet up the TCP/IP stack. 
  netif_input_fn input;

  netif_output_fn output;
  .. by ARP
  netif_linkoutput_fn linkoutput;

  netif_status_callback_fn status_callback;

*/

/*
....if LWIP_DHCP
  ** the DHCP client state information for this netif *
  struct dhcp *dhcp;
  struct udp_pcb *dhcps_pcb;    //dhcps
  dhcp_event_fn dhcp_event;

  struct autoip *autoip;

  char*  hostname;

  ** maximum transfer unit (in bytes) 
  u16_t mtu;
  ** number of bytes used in hwaddr
  u8_t hwaddr_len;
  ** link level hardware address of this interface 
  u8_t hwaddr[NETIF_MAX_HWADDR_LEN];
  ** flags (see NETIF_FLAG_ above) 
  u8_t flags;
  ** descriptive abbreviation 
  char name[2];
  ** number of this interface 
  u8_t num;
*/


/*
#if LWIP_SNMP
  ** link type (from "snmp_ifType" enum from snmp.h) *  u8_t link_type;
  ** (estimate) link speed *
  u32_t link_speed;
  ** timestamp at last change made (up/down) *
  u32_t ts;
  ** counters *
  u32_t ifinoctets;
  u32_t ifinucastpkts;
  u32_t ifinnucastpkts;
  u32_t ifindiscards;
  u32_t ifoutoctets;
  u32_t ifoutucastpkts;
  u32_t ifoutnucastpkts;
  u32_t ifoutdiscards;

*/

/*

#if LWIP_IGMP
  ** This function could be called to add or delete a entry in the multicast
      filter table of the ethernet MAC.*
  netif_igmp_mac_filter_fn igmp_mac_filter;
#endif * LWIP_IGMP *
#if LWIP_NETIF_HWADDRHINT
  u8_t *addr_hint;
#endif * LWIP_NETIF_HWADDRHINT *
*/



//=======================~~~~~~~~~~~~~~~~~~~~~----------------------------------------


/* API for application */

/*
	err_t netifapi_netif_add       ( struct netif *netif,
                                 ip_addr_t *ipaddr,
                                 ip_addr_t *netmask,
                                 ip_addr_t *gw,
                                 void *state,
                                 netif_init_fn init,
                                 netif_input_fn input);
 */

/*
	err_t netifapi_netif_set_addr  ( struct netif *netif,
                                 ip_addr_t *ipaddr,
                                 ip_addr_t *netmask,
                                 ip_addr_t *gw );
*/

/*
	err_t netifapi_netif_common    ( struct netif *netif,
                                 netifapi_void_fn voidfunc,
                                 netifapi_errt_fn errtfunc);

 */

// #define netifapi_netif_remove(n)      netifapi_netif_common(n, netif_remove, NULL)
// #define netifapi_netif_set_up(n)      netifapi_netif_common(n, netif_set_up, NULL)
// #define netifapi_netif_set_down(n)    netifapi_netif_common(n, netif_set_down, NULL)
// #define netifapi_netif_set_default(n) netifapi_netif_common(n, netif_set_default, NULL)
// #define netifapi_dhcp_start(n)        netifapi_netif_common(n, NULL, dhcp_start)
// #define netifapi_dhcp_stop(n)         netifapi_netif_common(n, dhcp_stop, NULL)
// #define netifapi_autoip_start(n)      netifapi_netif_common(n, NULL, autoip_start)
// #define netifapi_autoip_stop(n)       netifapi_netif_common(n, NULL, autoip_stop)



// Module function map
static const LUA_REG_TYPE net_if_map[] = {
  { LSTRKEY( "first_IF" ),             LFUNCVAL( first_IF ) },
  { LSTRKEY( "first_adr" ),             LFUNCVAL( first_adr ) },
  { LSTRKEY( "first_adr2" ),             LFUNCVAL( first_adr2 ) },


  { LSTRKEY( "__metatable" ),      LROVAL( net_if_map ) },
  { LNILKEY, LNILVAL }
};


// Register the module - NODEMCU_MODULE()  
NODEMCU_MODULE(NET_IF, "net_if", net_if_map, NULL);

