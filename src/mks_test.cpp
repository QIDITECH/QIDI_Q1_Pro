#include "../include/mks_test.h"
#include "../include/ui.h"
#include "../include/send_msg.h"
#include "../include/event.h"

extern int tty_fd;
extern int current_page_id;                    

// void *mks_test(void *arg) {
//     while (1) {
//         if (current_page_id == TJC_PAGE_MKS_TEST) {
//             if (true == testUSB()) {
//                 send_cmd_txt(tty_fd, "t2", "Okay");
//                 send_cmd_pco(tty_fd, "t2", "2047");
//                 sleep(2);
//             } else {
//                 send_cmd_txt(tty_fd, "t2", "fail");
//                 send_cmd_pco(tty_fd, "t2", "63488");
//             }

//             if (true == moko_test_func()) {
//                 send_cmd_txt(tty_fd, "t2", "okay");
//                 send_cmd_pco(tty_fd, "t2", "1024");
//                 sleep(2);
//             } else {
//                 send_cmd_txt(tty_fd, "t2", "fail");
//                 send_cmd_pco(tty_fd, "t2", "63488");
//             }

//             if (true == network_test_func()) {
//                 send_cmd_txt(tty_fd, "t3", "Okay");
//                 send_cmd_pco(tty_fd, "t3", "2047");
//             } else {
//                 send_cmd_txt(tty_fd, "t3", "fail");
//                 send_cmd_pco(tty_fd, "t3", "63488");
//             }
//         }
//         refresh_page_mks_test();
//         usleep(50000);
//     }
// }

bool testUSB()
{
    FILE *fp; 
	char buffer[1024]; 
	static int last_style = 0;
	
	fp=popen("lsusb | grep \"QinHeng Electronics\"", "r"); 
	fgets(buffer,sizeof(buffer),fp); 
	//printf("%s",buffer); 


    if(strstr(buffer, "QinHeng Electronics") != 0)
    {
        pclose(fp);
        return true;
    }
    else
    {
        pclose(fp);
        return false;
    }
}


bool moko_test_func()
{
    FILE *fp;
    char buffer[1024];
    static int last_style = 0;

    fp = popen("lsusb | grep \"OpenMoko\"", "r");
    fgets(buffer, sizeof(buffer), fp);

    if (strstr(buffer, "OpenMoko") != 0) {
        pclose(fp);
        return true;
    } else {
        pclose(fp);
        return false;
    }
}

bool network_test_func()
{
     FILE *fp; 
	char buffer[1024]; 
	static int last_style = 0;
	try  
    {  
	   
		fp=popen("ifconfig | grep \"inet 192.168.\"", "r"); 
		fgets(buffer,sizeof(buffer),fp); 

	   
		if(strstr(buffer, "inet 192.168.") != 0)
		{ 
            pclose(fp);
            return true;
            

		}
		else
		{
            pclose(fp);
            return false;
		}

		
	 }  
    catch (char *str)  
    {  pclose(fp);
        std::cout << "Standard exception: " << str << std::endl;  
    }  
	
	return 0;
}


