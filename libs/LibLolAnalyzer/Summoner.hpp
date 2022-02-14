#pragma once

#include <cpprest/details/basic_types.h>

namespace LibLolAnalyzer
{

	class Summoner
	{
	public:
		Summoner() = default;
        Summoner(utility::string_t id, utility::string_t name, uint8_t summoner_level, utility::string_t puuid);

		const utility::string_t& id() const;

		const utility::string_t& name() const;

		uint8_t summonerLevel() const;

        const utility::string_t& puuid() const;

	private:
        utility::string_t m_id;
		utility::string_t m_name;
		uint8_t m_summoner_level;
        utility::string_t m_puuid;
	};

}
