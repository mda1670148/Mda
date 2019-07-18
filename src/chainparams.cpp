// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2019 The Aratriton developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x00000fc630a7ded957563e9bd478de331a31c410091c4aa9b70525a24ba5958e"))
    (3800, uint256("0xad12f4e2c5fa732642927aadbb4b49d3556b4a0efd05a2ba5a113c488ee70a51"))
    (3801, uint256("0x9aa625c3e58deadb5f19e852c1cfc4333027353035afffcb273c7647aa6e1439"))
    (3802, uint256("0x04e511880d85514cce750a4048decfc58846693c84b8ee675e479846df788af1"))
    (4369, uint256("0xae4af5904ceaa922c47016ca4b2c7af94f0521b48f5c426146ea3a9dd43eb5fc"))
    (4871, uint256("0x3260405ed16456f08e72dcbed97ca2d9e7d807821e4a459c1d2c1690333d77c1"))
    (5122, uint256("0x585c0209e2d6d13daafefa21b678c56910170863feaa8b42e0ac54d4f167f175"))
    (5401, uint256("0x8564c65e7e313ae78feef0bed4179dc251cf9b3ed6f89d6d84e2556b94545d78"))
    (5561, uint256("0xeabd146a05813ef0da2d099a975e490ee416c434af5ecfc333e08ad7659beb7a"))
    (5736, uint256("0xf952fcc8088db8c21286e6e8d021af16eeba47a3777759d03cead2ca01769c12"))

;
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
	1563253130, // * UNIX timestamp of last checkpoint block
    19680,		// * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1562918400,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x1a;
        pchMessageStart[1] = 0x72;
        pchMessageStart[2] = 0xbc;
        pchMessageStart[3] = 0xe3;
        vAlertPubKey = ParseHex("0000048b6ba6ba6e7423fa5abd1a89e0a9a1463f88d332b44a5cb1a8b7ed2c1eaa335fc8dc4f012cb8241cc0bdafd6ca70c5f5448916e4e6f511bcd746ed57dc50");
        nDefaultPort = 9733;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Aratriton starting difficulty is 1 / 2^12
        //nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // ARA: 1 day
        nTargetSpacing = 1 * 60;  // ARA: 1 minute
        nMaturity = 61;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 2000000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 9999999;
        nZerocoinStartTime = 2081097200;
        nBlockEnforceSerialRange = 253; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 9999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nEnforceNewSporkKey = 1563015600; //!> Sporks signed after (GMT):  Saturday, July 13, 2019 11:00:00 AM must use the new spork key
        nRejectOldSporkKey = 1563080400; //!> Fully reject old spork key after (GMT): Sunday, July 14, 2019 5:00:00 AM

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "AratritonCore Masternode Coin.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		txNew.vout[0].SetEmpty();
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04b10e83b2703ccf321f7a3d62dd5841ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9a4b77b26d92428c61256cd42774babea0a073b2ed0c9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1562918400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 6218636;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000fc630a7ded957563e9bd478de331a31c410091c4aa9b70525a24ba5958e"));
        assert(genesis.hashMerkleRoot == uint256("0xf01e124f1c53d746223d5929c30e059de726a3ffea70c7b89fa24dd4faa4cca9"));

        vSeeds.push_back(CDNSSeedData("0", "dnsseeder.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("1", "explorer.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("2", "dnsseed1.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("3", "dnsseed2.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("4", "dnsseed3.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("5", "dnsseed4.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("6", "dnsseed5.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("7", "dnsseed6.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("8", "dnsseed7.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("9", "dnsseed8.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("10", "dnsseed9.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("11", "dnsseed10.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("12", "dnsseed11.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("13", "dnsseed12.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("14", "dnsseed13.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("15", "dnsseed14.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("16", "dnsseed15.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("17", "dnsseed16.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("18", "dnsseed17.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("19", "dnsseed18.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("20", "dnsseed19.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("21", "dnsseed20.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("22", "dnsseed21.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("23", "dnsseed22.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("24", "dnsseed23.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("25", "dnsseed24.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("26", "dnsseed25.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("27", "dnsseed26.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("28", "dnsseed27.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("29", "dnsseed28.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("30", "dnsseed29.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("31", "dnsseed30.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("32", "dnsseed31.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("33", "dnsseed32.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("34", "dnsseed33.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("35", "dnsseed34.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("36", "dnsseed35.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("37", "dnsseed36.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("38", "dnsseed37.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("39", "dnsseed38.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("40", "dnsseed39.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("41", "dnsseed40.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("42", "dnsseed41.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("43", "dnsseed42.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("44", "dnsseed43.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("45", "dnsseed44.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("46", "dnsseed45.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("47", "dnsseed46.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("48", "dnsseed47.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("49", "dnsseed48.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("50", "dnsseed49.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("51", "dnsseed50.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("52", "dnsseed51.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("53", "dnsseed52.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("54", "dnsseed53.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("55", "dnsseed54.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("56", "dnsseed55.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("57", "dnsseed56.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("58", "dnsseed57.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("59", "dnsseed58.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("60", "dnsseed59.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("61", "dnsseed60.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("62", "dnsseed61.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("63", "dnsseed62.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("64", "dnsseed63.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("65", "dnsseed64.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("66", "dnsseed65.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("67", "dnsseed66.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("68", "dnsseed67.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("69", "dnsseed68.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("70", "dnsseed69.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("71", "dnsseed70.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("72", "dnsseed71.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("73", "dnsseed72.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("74", "dnsseed73.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("75", "dnsseed74.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("76", "dnsseed75.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("77", "dnsseed76.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("78", "dnsseed77.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("79", "dnsseed78.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("80", "dnsseed79.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("81", "dnsseed80.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("82", "dnsseed81.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("83", "dnsseed82.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("84", "dnsseed83.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("85", "dnsseed84.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("86", "dnsseed85.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("87", "dnsseed86.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("88", "dnsseed87.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("89", "dnsseed88.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("90", "dnsseed89.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("91", "dnsseed90.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("92", "dnsseed91.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("93", "dnsseed92.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("94", "dnsseed93.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("95", "dnsseed94.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("96", "dnsseed95.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("97", "dnsseed96.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("98", "dnsseed97.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("99", "dnsseed98.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("100", "dnsseed99.aratriton.com"));
		vSeeds.push_back(CDNSSeedData("101", "dnsseed100.aratriton.com"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04d591ef0621758386743ccdd09f776196098b9c1eafab6b40e77c2c32c2733a0e76157ae648993d7af6ff4b651e143db28821caf335d12f0c06376ee1e71779b1";
        strSporkKeyOld = "04d591ef0621758386743ccdd09f776196098b9c1eafab6b40e77c2c32c2733a0e76157ae648993d7af6ff4b651e143db28821caf335d12f0c06376ee1e71779b1";
        strObfuscationPoolDummyAddress = "AcunUSx7AU2fNUm1Xrd7B3aphbLmULpfCD";
        nStartMasternodePayments = 1562949000; // Friday, July 12, 2019 4:30:00 PM GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zara to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x45;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0xba;
        vAlertPubKey = ParseHex("000010e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9");
        nDefaultPort = 51474;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Aratriton: 1 day
        nTargetSpacing = 1 * 60;  // Aratriton: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1562918400;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "aratriton-testnet.seed.fuzzbawls.pw"));
        vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "aratriton-testnet.seed2.fuzzbawls.pw"));
        vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net"));
        vSeeds.push_back(CDNSSeedData("88.198.192.110", "88.198.192.110"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet aratriton addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet aratriton script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet aratriton BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet aratriton BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet aratriton BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04A8B319388C0F8588D238B9941DC26B26D3F9465266B368A051C5C100F79306A557780101FE2192FE170D7E6DEFDCBEE4C8D533396389C0DAFFDBC842B002243C";
        strSporkKeyOld = "04348C2F50F90267E64FACC65BFDC9D0EB147D090872FB97ABAE92E9A36E6CA60983E28E741F8E7277B11A7479B626AC115BA31463AC48178A5075C5A9319D4A38";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        //nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Aratriton: 1 day
        nTargetSpacing = 1 * 60;        // Aratriton: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1562918400;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12345;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;
        //assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    //virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
