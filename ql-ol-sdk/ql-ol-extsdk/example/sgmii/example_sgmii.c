#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ql_sgmii.h"

static inline char *input_fgets(char *msg, ...)
{
	static char ptr[128] = { 0 };
	va_list ap;

	va_start(ap, msg);
	vprintf(msg, ap);
	va_end(ap);
	memset(ptr, 0, sizeof(ptr));
	fgets(ptr, sizeof(ptr), stdin);
	if(strlen(ptr) > 0) {
		if('\n' == ptr[strlen(ptr) - 1]) {
			ptr[strlen(ptr) - 1] = '\0';
		}
	}
	return ptr;
}

int main(int argc, char **argv)
{
	char *sptr = NULL;
	ql_sgmii_autoneg_e autoneg;
	ql_sgmii_speed_e speed;
	ql_sgmii_duplex_e duplex;

	while(1) {
		printf("\ncase 1: Enable SGMII");
		printf("\ncase 2: Disable SGMII");
		printf("\ncase 3: Configure SGMII");
		printf("\ncase 4: Show SGMII configuraction");
		printf("\ncase 100: exit");
		
		sptr = input_fgets("\nPlease input case: ");
		switch(atoi(sptr)) {
			case 1:
			{
				if(ql_sgmii_enable()) {
					printf("\nEnable SGMII Module failure");
					break;
				}
				printf("\nEnable SGMII Module Success");
				break;
			}
			case 2:
			{
				if(ql_sgmii_disable()) {
					printf("\nDisable SGMII Module failure");
					break;
				}
				printf("\nDisable SGMII Module Success");
				break;
			}
			case 3:
			{
				sptr = input_fgets("\nPlease input SGMII auto-negotiation(1-ON/0-OFF): ");
				autoneg = atoi(sptr);
				if(QL_SGMII_AUTONEG_OFF != autoneg && QL_SGMII_AUTONEG_ON != autoneg) {
					printf("\nInvalid autoneg");
					break;
				}
				if(ql_sgmii_autoneg_set(autoneg)) {
					printf("\nset autoneg failure");
					break;
				}
				if(QL_SGMII_AUTONEG_OFF == autoneg) {
					sptr = input_fgets("\nPlease input SGMII speed(0-10M/1-100M): ");
					speed = atoi(sptr);
					if(speed < QL_SGMII_SPEED_10MHZ || speed > QL_SGMII_SPEED_100MHZ) {
						printf("\nInvalid speed");
						break;
					}
					if(ql_sgmii_speed_set(speed)) {
						printf("\nset speed failure");
						break;
					}
					sptr = input_fgets("\nPlease input SGMII duplex(0-FULL/1-HALF): ");
					duplex = atoi(sptr);
					if(0 != duplex && 1 != duplex) {
						printf("\nInvalid duplex");
						break;
					}
					if(ql_sgmii_duplex_set(duplex)) {
						printf("\nset duplex failure");
						break;
					}
				}
				break;
			}
			case 4:
			{
				struct ql_sgmii_info info = { 0 };
				char speed_msg[][32] = {"10MHZ", "100MHZ", "1000MHZ"};

				autoneg = QL_SGMII_AUTONEG_OFF;
				if(ql_sgmii_autoneg_get(&autoneg)) {
					printf("\nget setting autoneg failure");
					break;
				}
				
				speed = QL_SGMII_SPEED_10MHZ;
				if(ql_sgmii_speed_get(&speed)) {
					printf("\nget setting speed failure");
					break;
				}

				duplex = QL_SGMII_DUPLEX_HALF;
				if(ql_sgmii_duplex_get(&duplex)) {
					printf("\nget setting duplex failure");
					break;
				}
				if(ql_smgii_info_get(&info)) {
					printf("\nget sgmii info failure");
					break;
				}
				printf("\n get setting speed: %s, running speed %s", speed_msg[speed], speed_msg[info.speed]);
				printf("\n get setting duplex: %s, running duplex %s",
				       QL_SGMII_DUPLEX_FULL == duplex ? "FULL" : "HALF",
				       QL_SGMII_DUPLEX_FULL == info.duplex ? "FULL" : "HALF");
				printf("\n tx packets %lu"
				       "\n tx bytes %lu"
				       "\n rx packets %lu"
				       "\n rx bytes %lu", info.tx_pkts, info.tx_bytes, info.rx_pkts, info.rx_bytes);
				break;
			}
			case 100:
				return 0;
			default:
				printf("\nInvalid Case");
				break;
		}
	}
}
