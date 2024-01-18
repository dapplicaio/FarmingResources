#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>
#include "atomicassets.hpp"


using namespace eosio;

class [[eosio::contract]] game : public contract 
{
  public:
    using contract::contract;


    // listening atomicassets transfer
    [[eosio::on_notify("atomicassets::transfer")]] 
    void receive_asset_transfer
    (
      const name& from,
      const name& to,
      std::vector <uint64_t>& asset_ids,
      const std::string& memo
    );


    [[eosio::action]]
    void claim(const name& owner, const uint64_t& farmingitem);


  private:

  //scope: owner
  struct [[eosio::table]] staked_j
  {
    uint64_t              asset_id; // item
    std::vector<uint64_t> staked_items; // farming items

    uint64_t primary_key() const { return asset_id; }
  };
  typedef multi_index< "staked"_n, staked_j > staked_t;

  //scope: owner
  struct [[eosio::table]] resources_j
  {
    uint64_t    key_id;
    float       amount;
    std::string resource_name;

    uint64_t primary_key() const { return key_id; }
  };
  typedef multi_index< "resources"_n, resources_j > resources_t;

  const uint64_t stringToUint64(const std::string& str);

  void stake_farmingitem(const name& owner, const uint64_t& asset_id);
  void stake_items(const name& owner, const uint64_t& farmingitem, const std::vector<uint64_t>& items_to_stake);

  void increase_owner_resources_balance(const name& owner, const std::map<std::string, float>& resources);

  const std::pair<std::string, float> claim_item(atomicassets::assets_t::const_iterator& assets_itr, const uint8_t& upgrade_percentage, const uint32_t& time_now);


  // get mutable data from NFT
  atomicassets::ATTRIBUTE_MAP get_mdata(atomicassets::assets_t::const_iterator& assets_itr);
  // get immutable data from template of NFT
  atomicassets::ATTRIBUTE_MAP get_template_idata(const int32_t& template_id, const name& collection_name);
  // update mutable data of NFT
  void update_mdata(atomicassets::assets_t::const_iterator& assets_itr, const atomicassets::ATTRIBUTE_MAP& new_mdata, const name& owner);
};
