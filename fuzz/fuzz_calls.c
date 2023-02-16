#include <stdint.h>
#include <string.h>

#include "../src/angle_plugin.h"



// List of selectors supported by this plugin.
// EDIT THIS: Adapt the variable names and change the `0x` values to match your selectors.
static const uint8_t MINT_SELECTOR[SELECTOR_SIZE] = {0x07, 0x4e, 0xe4, 0x46};
static const uint8_t BURN_SELECTOR[SELECTOR_SIZE] = {0x6d, 0x10, 0x42, 0x16};
static const uint8_t SLP_DEPOSIT_SELECTOR[SELECTOR_SIZE] = {0x2e, 0x2d, 0x29, 0x84};
static const uint8_t SLP_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x23, 0xe1, 0x03, 0xa8};
static const uint8_t OPEN_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0xa6, 0x2b, 0x2a, 0x3d};
static const uint8_t ADD_TO_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0x24, 0xd8, 0x3b, 0x79};
static const uint8_t CLOSE_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0xe5, 0x23, 0x4c, 0x7a};
static const uint8_t GET_REWARD_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0x1c, 0x4b, 0x77, 0x4b};

// Array of all the different angle selectors. Make sure this follows the same order as the
// enum defined in `angle_plugin.h`
// EDIT THIS: Use the names of the array declared above.
const uint8_t *const ANGLE_SELECTORS[NUM_SELECTORS] = {MINT_SELECTOR,
                                                       BURN_SELECTOR,
                                                       SLP_DEPOSIT_SELECTOR,
                                                       SLP_WITHDRAW_SELECTOR,
                                                       OPEN_PERPETUAL_SELECTOR,
                                                       ADD_TO_PERPETUAL_SELECTOR,
                                                       CLOSE_PERPETUAL_SELECTOR,
                                                       GET_REWARD_PERPETUAL_SELECTOR};


// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters) {
    switch (message) {
        case ETH_PLUGIN_INIT_CONTRACT:
            handle_init_contract(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_PARAMETER:
            handle_provide_parameter(parameters);
            break;
        case ETH_PLUGIN_FINALIZE:
            handle_finalize(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_INFO:
            handle_provide_token(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_ID:
            handle_query_contract_id(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            handle_query_contract_ui(parameters);
            break;
        default:
            PRINTF("Unhandled message %d\n", message);
            break;
    }
}

// from: eth_plugin_interface.h
//typedef enum {
//
//    ETH_PLUGIN_INIT_CONTRACT = 0x0101,
//    ETH_PLUGIN_PROVIDE_PARAMETER = 0x0102,
//    ETH_PLUGIN_FINALIZE = 0x0103,
//    ETH_PLUGIN_PROVIDE_INFO = 0x0104,
//    ETH_PLUGIN_QUERY_CONTRACT_ID = 0x0105,
//    ETH_PLUGIN_QUERY_CONTRACT_UI = 0x0106,
//    ETH_PLUGIN_CHECK_PRESENCE = 0x01FF
//
//} eth_plugin_msg_t;

// fuzz target
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    const int max_len = 5;
    if (size > 0 && size < max_len) {
        int message = 0x0100 + data[0];

        // TODO need to pass actual eth structures based on each case
        /*
         * ethPluginInitContract_t
         * ethPluginProvideParameter_t
         * ethPluginFinalize_t
         * ethPluginProvideInfo_t
         * ethQueryContractID_t
         * ethQueryContractUI_t
        */


        long parameters = 0;
        if (size > 1) {
            parameters += data[1];
        }
        if (size > 2) {
            parameters += (data[2] << 8);
        }
        if (size > 3) {
            parameters += (data[3] << 16);
        }
        if (size > 4) {
            parameters += (data[4] << 24);
        }

        dispatch_plugin_calls(message, (void *)parameters);
    } else {
        return 0;
    }

    return 0;
}
