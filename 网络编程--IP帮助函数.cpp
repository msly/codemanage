网络编程--IP帮助函数

//from http://www.cnblogs.com/Clingingboy/archive/2011/06/21/2086348.html

一.GetNetworkParams获取本地电脑的网络参数
获取一个FIXED_INFO的结构体 

typedef struct {
    char HostName[MAX_HOSTNAME_LEN + 4] ;
    char DomainName[MAX_DOMAIN_NAME_LEN + 4];
    PIP_ADDR_STRING CurrentDnsServer;
    IP_ADDR_STRING DnsServerList;
    UINT NodeType;
    char ScopeId[MAX_SCOPE_ID_LEN + 4];
    UINT EnableRouting;
    UINT EnableProxy;
    UINT EnableDns;
} FIXED_INFO_W2KSP1, *PFIXED_INFO_W2KSP1;
#if (NTDDI_VERSION >= NTDDI_WIN2KSP1)
typedef  FIXED_INFO_W2KSP1 FIXED_INFO;
typedef  FIXED_INFO_W2KSP1 *PFIXED_INFO;
#endif
二.网络接口管理
1.GetNumberOfInterfaces获取接口数量

DWORD GetNumberOfInterfaces( 
  __out  PDWORD pdwNumIf 
);

2.GetAdaptersInfo返回本地网络适配器列表

DWORD GetAdaptersInfo( 
  __out    PIP_ADAPTER_INFO pAdapterInfo, 
  __inout  PULONG pOutBufLen 
);

int main()
{
    PIP_INTERFACE_INFO pInfo = 
            (PIP_INTERFACE_INFO)::GlobalAlloc(GPTR, sizeof(IP_INTERFACE_INFO));
    ULONG ulOutBufLen = sizeof(IP_INTERFACE_INFO);

    // 如果上面申请的内存不够的话，再重新申请
    if(::GetInterfaceInfo(pInfo, &ulOutBufLen) == ERROR_INSUFFICIENT_BUFFER)
    {
        ::GlobalFree(pInfo);
        pInfo = (PIP_INTERFACE_INFO)::GlobalAlloc(GPTR, ulOutBufLen);
    }

    // 再次调用GetInterfaceInfo来获取我们实际需要的数据
    if(::GetInterfaceInfo(pInfo, &ulOutBufLen) == NO_ERROR)
    {
        printf(" \tAdapter Name: %ws\n", pInfo->Adapter[0].Name);
        printf(" \tAdapter Index: %ld\n", pInfo->Adapter[0].Index);
        printf(" \tNum Adapters: %ld\n", pInfo->NumAdapters);
    }
    else
    {
        printf(" GetInterfaceInfo() failed \n");
    }

    ::GlobalFree(pInfo);
    return 0;
}
三.IP地址管理
获取IP地址列表、添加和删除IP地址 
GetIpAddrTable、AddIPAddress、DeleteIPAddress

int main()
{
        // 首先调用GetIpAddrTable函数获取一个适配器
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;

    pIPAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));

    // 获取所需的内存
    if(::GetIpAddrTable(pIPAddrTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
    {
        ::GlobalFree(pIPAddrTable);
        pIPAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, dwSize);
    }

    // 再次调用GetIpAddrTable获取实际我们想要的数据
    if(::GetIpAddrTable(pIPAddrTable, &dwSize, FALSE) == NO_ERROR)
    {
        // 打印出适配器信息
        printf(" Address: %ld\n", pIPAddrTable->table[0].dwAddr);
        printf(" Mask:    %ld\n", pIPAddrTable->table[0].dwMask);
        printf(" Index:   %ld\n", pIPAddrTable->table[0].dwIndex);
        printf(" BCast:   %ld\n", pIPAddrTable->table[0].dwBCastAddr);
        printf(" Reasm:   %ld\n", pIPAddrTable->table[0].dwReasmSize);
    }    
    else
    {
        printf(" GetIpAddrTable() failed \n");
    }
    
    ::GlobalFree(pIPAddrTable);
    
    // 我们将要添加的IP和mast
    UINT iaIPAddress;
    UINT imIPMask;
    
    iaIPAddress = inet_addr("192.168.0.27");
    imIPMask = inet_addr("255.255.255.0");
    
    // 返回的句柄
    ULONG NTEContext = 0;
    ULONG NTEInstance = 0;
    
    // 向第一个适配器添加IP地址
    DWORD dwRet;
    dwRet = ::AddIPAddress(iaIPAddress, imIPMask, 
                pIPAddrTable->table[0].dwIndex, &NTEContext, &NTEInstance);
    if(dwRet == NO_ERROR) 
    {
        printf(" IP address added.\n");
    }
    else 
    {
        printf(" AddIPAddress failed. \n");
        LPVOID lpMsgBuf;
        // 调用失败，打印出为什么失败
        if (::FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM | 
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwRet,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL )) 
        {
            printf(" Error: %s ", lpMsgBuf);
        }
        ::LocalFree(lpMsgBuf);
    }
    
    // 删除上面在第一个适配器上添加的IP地址
    dwRet = ::DeleteIPAddress(NTEContext);
    if(dwRet == NO_ERROR) 
    {
        printf(" IP Address Deleted.\n");
    }
    else 
    {        
        printf(" DeleteIPAddress failed.\n");
    }
    
    return 0;
}
四.获取网络状态信息
1.获取TCP和UDP连接表(GetTcpTable,GetUdpTable)

PMIB_UDPTABLE MyGetUdpTable(BOOL bOrder)
{
    PMIB_UDPTABLE pUdpTable = NULL;
    DWORD dwActualSize = 0;

    // 查询所需缓冲区的大小
    if(::GetUdpTable(pUdpTable, &dwActualSize, bOrder) == ERROR_INSUFFICIENT_BUFFER)
    {
        // 为MIB_UDPTABLE结构申请内存
        pUdpTable = (PMIB_UDPTABLE)::GlobalAlloc(GPTR, dwActualSize);
        // 获取UDP监听表
        if(::GetUdpTable(pUdpTable, &dwActualSize, bOrder) == NO_ERROR)
            return pUdpTable;
        ::GlobalFree(pUdpTable);
    }
    return NULL;
}

void MyFreeUdpTable(PMIB_UDPTABLE pUdpTable)
{
    if(pUdpTable != NULL)
        ::GlobalFree(pUdpTable);
}
// 打印UDP监听表信息
PMIB_UDPTABLE pUdpTable = MyGetUdpTable(TRUE);
if(pUdpTable != NULL)
{
    struct in_addr inadLocal;
    printf("UDP TABLE\n");

    printf("%20s %10s\n", "Loc Addr", "Loc Port");
    for (UINT i = 0; i < pUdpTable->dwNumEntries; ++i)
    {
        inadLocal.s_addr = pUdpTable->table[i].dwLocalAddr;
        // 打印出此入口的信息
        printf("%20s %10u \n", 
            inet_ntoa(inadLocal), ntohs((unsigned short)(0x0000FFFF & pUdpTable->table[i].dwLocalPort)));
    }

    MyFreeUdpTable(pUdpTable);
}


五.获取相关统计数据
相关的函数GetxxxStatistics(GetIpStatistics,GetIcmpStatistics,GetTcpStatistics,GetUdpStatistics)

// 获取UDP统计数据
MIB_UDPSTATS UdpStats;
if(::GetUdpStatistics(&UdpStats) == NO_ERROR)
{
    MIB_UDPSTATS *pStats = &UdpStats;
    printf("\nUDP Statistics\n");
    printf("\
              dwInDatagrams      = %lu\n\
              dwNoPorts          = %lu\n\
              dwInErrors         = %lu\n\
              dwOutDatagrams     = %lu\n\
              dwNumAddrs         = %lu\n",
              pStats->dwInDatagrams,
              pStats->dwNoPorts,
              pStats->dwInErrors,
              pStats->dwOutDatagrams,
              pStats->dwNumAddrs);
}
六.路由表管理
1.GetIpForwardTable获取路由表

PMIB_IPFORWARDTABLE MyGetIpForwardTable(BOOL bOrder)
{
    PMIB_IPFORWARDTABLE pIpRouteTab = NULL;
    DWORD dwActualSize = 0;
    
    // 查询所需缓冲区的大小
    if(::GetIpForwardTable(pIpRouteTab, &dwActualSize, bOrder) == ERROR_INSUFFICIENT_BUFFER)
    {
        // 为MIB_IPFORWARDTABLE结构申请内存
        pIpRouteTab = (PMIB_IPFORWARDTABLE)::GlobalAlloc(GPTR, dwActualSize);
        // 获取路由表
        if(::GetIpForwardTable(pIpRouteTab, &dwActualSize, bOrder) == NO_ERROR)
            return pIpRouteTab;
        ::GlobalFree(pIpRouteTab);
    }
    return NULL;
}
void PrintIpForwardTable()
{
    PMIB_IPFORWARDTABLE pIpRouteTable = MyGetIpForwardTable(TRUE);
    if(pIpRouteTable != NULL)
    {
        DWORD i, dwCurrIndex;
        struct in_addr inadDest;
        struct in_addr inadMask;
        struct in_addr inadGateway;  
        PMIB_IPADDRTABLE pIpAddrTable = NULL;
        
        char szDestIp[128];
        char szMaskIp[128];
        char szGatewayIp[128];

        printf("Active Routes:\n\n");
        
        printf("  Network Address          Netmask  Gateway Address        Interface  Metric\n");
        for (i = 0; i < pIpRouteTable->dwNumEntries; i++)
        {
            dwCurrIndex = pIpRouteTable->table[i].dwForwardIfIndex;
    
            // 目的地址
            inadDest.s_addr = pIpRouteTable->table[i].dwForwardDest;
            // 子网掩码
            inadMask.s_addr = pIpRouteTable->table[i].dwForwardMask;
            // 网关地址
            inadGateway.s_addr = pIpRouteTable->table[i].dwForwardNextHop;
            
            strcpy(szDestIp, inet_ntoa(inadDest));
            strcpy(szMaskIp, inet_ntoa(inadMask));
            strcpy(szGatewayIp, inet_ntoa(inadGateway));
            printf("  %15s %16s %16s %16d %7d\n", 
                szDestIp, 
                szMaskIp, 
                szGatewayIp, 
                pIpRouteTable->table[i].dwForwardIfIndex,    // 可以在此调用GetIpAddrTable获取索引对应的IP地址
                pIpRouteTable->table[i].dwForwardMetric1);
        }
        MyFreeIpForwardTable(pIpRouteTable);
    }
}
2.添加、删除、修改路由
CreateIpForwardEntry、DeleteIpForwardEntry、SetIpForwardEntry

七.ARP表管理
1.获取IP地址到适配器的映射关系，即ARP表(GetIpNetTable)

BOOL InterfaceIdxToInterfaceIp(PMIB_IPADDRTABLE pIpAddrTable, DWORD dwIndex, char str[])
{
    char* szIpAddr;

    if(pIpAddrTable == NULL ||  str == NULL)
        return FALSE;
    str[0] = '\0';
    // 遍历IP地址表，查找索引dwIndex对应的IP地址
    for(DWORD dwIdx = 0; dwIdx < pIpAddrTable->dwNumEntries; dwIdx++)
    {
        if(dwIndex == pIpAddrTable->table[dwIdx].dwIndex)
        {
            // 以字符串的形式返回查询结果
            szIpAddr = inet_ntoa(*((in_addr*)&pIpAddrTable->table[dwIdx].dwAddr));
            if(szIpAddr)
            {
                strcpy(str, szIpAddr);
                return TRUE;
            }
            else
                return FALSE;
        }
    }
    return FALSE;
}