/**
* Copyright (C) 2015 Yingkit Tse
* Special Thanks to Shindo
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* This is Personalized Version of EasyDrcom.
*  Made for Yingkit Tse
*  Made by Yingkit Tse
*/


#ifndef __INCLUDE_DRCOM_DEALER_BASE_U64__
#define __INCLUDE_DRCOM_DEALER_BASE_U64__

class drcom_dealer_u64 : public drcom_dealer_base {
public:
	drcom_dealer_u64(std::vector<uint8_t> local_mac, std::string local_ip, std::string username, std::string password,
		std::string gateway_ip, uint32_t gateway_port, std::string hostname, std::string kernel_version
		) : local_mac(local_mac), local_ip(str_ip_to_vec(local_ip)),
		hostname(hostname), kernel_version(kernel_version),
		username(username), password(password),
		total_time(0), total_flux(0), balance(0), online_time(0), pkt_id(0), misc1_flux(0), misc3_flux(0),
		udp(gateway_ip, gateway_port, local_ip)
	{}

	int start_request()
	{
		U64_LOG_INFO("Start Request." << std::endl);
		std::vector<uint8_t> pkt_data;
		pkt_data.push_back(0x07); // Code
		pkt_data.push_back(pkt_id);
		pkt_data.insert(pkt_data.end(), { 0x08, 0x00, 0x01, 0x00, 0x00, 0x00 });

		challenge.clear();
		used_flux.clear();

		U64_LOG_SEND_DUMP

		auto handler_success = [&](std::vector<uint8_t> recv) -> int {
			U64_LOG_RECV_DUMP("Start Request");

			if (recv[2] != 0x10 && recv[3] != 0x00) // Start Response
				return -1;

			U64_LOG_INFO("Gateway return: Start Response." << std::endl);
			pkt_id++;
			U64_LOG_DBG("next packet id = " << (int)pkt_id << std::endl);
			challenge.resize(4);
			memcpy(&challenge[0], &recv[4], 4); // Challenge
			used_flux.resize(4);
			memcpy(&used_flux[0], &recv[8], 4);
			return 0;
		};

		U64_HANDLE_ERROR("Start Request");
		U64_PERFORM("Start Request");
	}

	int send_login_auth()
	{
		if (challenge.empty()) return -1;
		U64_LOG_INFO("Send Login Auth." << std::endl);
		U64_LOG_DBG("username = " << username << ", password = " << password << std::endl);

		auth_info.clear();
		svr_port.clear();
		cli_port.clear();

		std::vector<uint8_t> pkt_data;
		pkt_data.push_back(0x07); // Code
		pkt_data.push_back(pkt_id);
		pkt_data.insert(pkt_data.end(), { 0xf4, 0x00, 0x03, 0x0b }); // Packet Type
		pkt_data.insert(pkt_data.end(), local_mac.begin(), local_mac.end());
		pkt_data.insert(pkt_data.end(), local_ip.begin(), local_ip.end());
		pkt_data.insert(pkt_data.end(), { 0x02, 0x22, 0x00, 0x68 });
		pkt_data.insert(pkt_data.end(), { 0x5a, 0xa1, 0x80, 0x02, 0x90, 0xb2, 0x48, 0x15 });

		pkt_data.insert(pkt_data.end(), 4, 0x00);

		/************** UserName && Hostname ****************/
		std::vector<uint8_t> info_block(76, 0); // fixed length = 76
		memcpy(&info_block[0], &username[0], username.length() <= 76 ? username.length() : 76);
		memcpy(&info_block[username.length()], &hostname[0], hostname.length() <= 76 - username.length() ? hostname.length() : 76 - username.length());
		pkt_data.insert(pkt_data.end(), info_block.begin(), info_block.end());

		std::vector<uint8_t> kernel_version_block(64, 0); // fixed length = 64
		memcpy(&kernel_version_block[0], &kernel_version[0], kernel_version.length() <= 64 ? kernel_version.length() : 64);
		pkt_data.insert(pkt_data.end(), kernel_version_block.begin(), kernel_version_block.end());

		pkt_data.insert(pkt_data.end(), { 0x64, 0x36, 0x35, 0x39, 0x32, 0x65, 0x63, 0x63, 0x33, 0x35 });
		pkt_data.insert(pkt_data.end(), { 0x35, 0x34, 0x30, 0x65, 0x37, 0x32, 0x63, 0x37 });
		pkt_data.insert(pkt_data.end(), { 0x36, 0x65, 0x65, 0x35, 0x38, 0x64, 0x65, 0x36 });
		pkt_data.insert(pkt_data.end(), { 0x65, 0x35, 0x65, 0x38, 0x37, 0x65, 0x61, 0x62 });
		pkt_data.insert(pkt_data.end(), { 0x66, 0x63, 0x62, 0x38, 0x62, 0x32 });
		pkt_data.insert(pkt_data.end(), 24, 0x00);

		U64_LOG_SEND_DUMP

			auto handler_success = [&](std::vector<uint8_t> recv) -> int {
			U64_LOG_RECV_DUMP("Send Login Auth");

			if (recv[0] != 0x07)
				return -1;

			if (recv[2] != 0x30 && recv[3] != 0x00)
				return -1;
			else
			{
				U64_LOG_INFO("Gateway return: Login Succeed.");
				pkt_id++;
				U64_LOG_DBG("next packet id = " << (int)pkt_id << std::endl);
				auth_info.insert(auth_info.end(), recv.begin() + 16, recv.begin() + 16 + 16);
				svr_port.insert(svr_port.end(), auth_info.begin() + 9, auth_info.begin() + 10);
				cli_port.insert(cli_port.end(), auth_info.begin() + 15, auth_info.begin() + 16);

				return 0;
			}
		};

		U64_HANDLE_ERROR("Send Login Auth.");
		U64_PERFORM("Send Login Auth.");
	}

	int send_alive_pkt1()
	{
		U64_LOG_INFO("Send Alive Packet 1." << std::endl);

		std::vector<uint8_t> pkt_data;
		pkt_data.push_back(0x07); // Code
		pkt_data.push_back(pkt_id);
		pkt_data.insert(pkt_data.end(), { 0x28, 0x00 }); // Type
		pkt_data.insert(pkt_data.end(), { 0x0B, 0x01 }); // Step
		pkt_data.insert(pkt_data.end(), { 0xDC, 0x02 }); // Fixed Unknown
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00 }); // Unkown
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00, 0x00, 0x00 }); // some time
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00 }); // Fixed Unknown

		// some flux
		pkt_data.insert(pkt_data.end(), 4, 0x00);
		memcpy(&pkt_data[16], &misc1_flux, 4);

		pkt_data.insert(pkt_data.end(), 8, 0x00); // Fixed Unknown, 0x00 *8
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00, 0x00, 0x00 }); // Client IP (Fixed: 0.0.0.0)
		pkt_data.insert(pkt_data.end(), 8, 0x00); // Fixed Unknown, 0x00 *8

		U64_LOG_SEND_DUMP

			auto handler_success = [&](std::vector<uint8_t> recv) -> int {
			U64_LOG_RECV_DUMP("Alive Packet 1");

			if (recv[0] != 0x07) // Misc
				return -1;

			if (recv[5] == 0x06) // File
			{
				U64_LOG_INFO("Received 'Misc, File', Send Keep Alive Packet 1 again." << std::endl);
				return send_alive_pkt1();
			}
			else
			{
				U64_LOG_INFO("Gateway return: Response for Alive Packet 1." << std::endl);

				pkt_id++;
				U64_LOG_DBG("next packet id = " << (int)pkt_id << std::endl);

				memcpy(&misc3_flux, &recv[16], 4);
				return 0;
			}
		};

		U64_HANDLE_ERROR("Send Alive Packet 1");
		U64_PERFORM("Send Alive Packet 1");
	}

	int send_alive_pkt2()
	{
		U64_LOG_INFO("Send Alive Packet 2." << std::endl);

		std::vector<uint8_t> pkt_data;
		pkt_data.push_back(0x07); // Code
		pkt_data.push_back(pkt_id);
		pkt_data.insert(pkt_data.end(), { 0x28, 0x00 }); // Type
		pkt_data.insert(pkt_data.end(), { 0x0B, 0x03 }); // Step
		pkt_data.insert(pkt_data.end(), { 0xDC, 0x02 }); // Fixed Unknown
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00 }); // Unkown
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00, 0x00, 0x00 }); // some time
		pkt_data.insert(pkt_data.end(), { 0x00, 0x00 }); // Fixed Unknown

		// some flux
		pkt_data.insert(pkt_data.end(), 4, 0x00);
		memcpy(&pkt_data[16], &misc3_flux, 4);

		pkt_data.insert(pkt_data.end(), 8, 0x00); // Fixed Unknown, 0x00 *8
		pkt_data.insert(pkt_data.end(), local_ip.begin(), local_ip.end()); // Client IP
		pkt_data.insert(pkt_data.end(), 8, 0x00); // Fixed Unknown, 0x00 *8

		U64_LOG_SEND_DUMP

			auto handler_success = [&](std::vector<uint8_t> recv) -> int {
			U64_LOG_RECV_DUMP("Alive Packet 2");

			if (recv[0] != 0x07 && recv[5] != 0x04) // Misc 4
				return -1;

			U64_LOG_INFO("Gateway return: Response for Alive Packet 2." << std::endl);

			pkt_id++;
			U64_LOG_DBG("next packet id = " << (int)pkt_id << std::endl);

			memcpy(&misc1_flux, &recv[16], 4);
			return 0;
		};

		U64_HANDLE_ERROR("Send Alive Packet 2");
		U64_PERFORM("Send Alive Packet 2");
	}

	int send_alive_request()
	{
		U64_LOG_INFO("Alive, Client to Server per 20s." << std::endl);

		std::vector<uint8_t> pkt_data;
		pkt_data.push_back(0xFF); // Code

		/********************** MD5A **************************/
		// Function_MD5A = MD5(code + type + Challenge + Password)
		pkt_data.insert(pkt_data.end(), { 0x46, 0xa1, 0x07, 0x55, 0x11, 0x8b, 0x53, 0x96, 0x4d, 0x62, 0x06, 0x42, 0x12, 0x48, 0x52, 0x3f });

		pkt_data.insert(pkt_data.end(), 3, 0x00);
		pkt_data.insert(pkt_data.end(), { 0x44, 0x72, 0x63, 0x6f }); // DrcomFlag(Drco)
		pkt_data.insert(pkt_data.end(), { 0xc0, 0xa8, 0x7f, 0x81 }); // Server IP 192.168.127.129
		pkt_data.insert(pkt_data.end(), { 0x8a, 0x35 }); // Server Port Unknow
		pkt_data.insert(pkt_data.end(), local_ip.begin(), local_ip.end()); // Client IP
		pkt_data.insert(pkt_data.end(), { 0x01, 0x91 }); // Client Port Unknow
		pkt_data.insert(pkt_data.end(), 2, 0x00); // Some time

		uint16_t now_time = (uint16_t)(time(NULL) % 86400);

		U64_LOG_SEND_DUMP

			auto handler_success = [&](std::vector<uint8_t> recv) -> int {
			U64_LOG_RECV_DUMP("Send Alive Request");

			if (recv[0] != 0x07 && recv[5] != 0x00 && recv[2] != 0x10) // Response for Alive
				return -1;

			U64_LOG_INFO("Gateway return: Response for alive." << std::endl);

			// Captured
			memcpy(&online_time, &recv[32], 4);
			memcpy(&total_time, &recv[44], 4);
			memcpy(&total_flux, &recv[48], 4);
			memcpy(&balance, &recv[52], 4);
			memcpy(&time_balance, &recv[56], 4);

#ifdef OPENWRT
			// network order on openwrt
			online_time = TO_LITTLE_ENDIAN(online_time);
			total_time = TO_LITTLE_ENDIAN(total_time);
			total_flux = TO_LITTLE_ENDIAN(total_flux);
			balance = TO_LITTLE_ENDIAN(balance);
			time_balance = TO_LITTLE_ENDIAN(time_balance);
#endif

			U64_LOG_INFO("Keep Alive succeeded! Timestamp = " << now_time << ", user info:" << std::endl);
			U64_LOG_INFO("Online Time: " << online_time << " Seconds, Used Time: " << total_time << " Minutes, Used Flux: " << (total_flux & 0x0FFFFFFFF) / 1024.0 << " MB, Balance: " << (balance & 0x0FFFFFFFF) / 10000.0 << " RMB" << std::endl);

			return 0;
		};

		U64_HANDLE_ERROR("Send Alive Request");
		U64_PERFORM("Send Alive Request");
	}

	int recv_svr_message()
	{
		auto handler_success = [&](std::vector<uint8_t> recv) -> int {
			U64_LOG_RECV_DUMP("Message, Server Infomation");

			if (recv[0] != 0x4d) // Message
				return -1;

			U64_LOG_INFO("Gateway return: Server Message." << std::endl);
			std::string str;
			str.resize(recv.size() - 4);
			memcpy(&str[0], &recv[recv.size() + 4], recv.size() - 4);
			std::cout << str << std::endl;

			return 0;
		};

		U64_HANDLE_ERROR("Receive Gateway Message");
		U64_RECEIVE("Receive Gateway Message");
	}


private:
	udp_dealer udp;

	// Const
	std::vector<uint8_t> local_mac, local_ip;
	std::string username, password, hostname, kernel_version;

	// Send Login Auth
	std::vector<uint8_t> challenge;
	std::vector<uint8_t> used_flux;

	// Used by Alive
	std::vector<uint8_t> login_md5_a;

	// Recv from Success
	std::vector<uint8_t> auth_info;
	std::vector<uint8_t> svr_port;
	std::vector<uint8_t> cli_port;

	// Update from Succes & Alive
	uint32_t total_time, total_flux, balance, online_time, time_balance;

	// Send Misc1, 3
	uint8_t pkt_id;
	uint32_t misc1_flux, misc3_flux;
};

#endif