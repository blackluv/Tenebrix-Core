// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2022 The Dogecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "\"nytimes.com 10/1/2011 - Police Arrest Over 700 Protesters on Brooklyn Bridge\"";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        // Not used in Tenebrix
        consensus.nSubsidyHalvingInterval = 100000;

        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;

        // After deployments are activated we can change it
        consensus.BIP34Hash = uint256S("0xbfe98ccd4064069fdbd98e6fbc464683872fabd1659e06e9c02b2705d5f32bd3");
        consensus.BIP65Height = 0x210c;
        consensus.BIP66Height = 0x210c;

        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 3 * 60 * 60; // pre-digishield: 3 hours
        consensus.nPowTargetSpacing = 5 * 60; // 5 minute
        consensus.nCoinbaseMaturity = 70;
        consensus.fPowNoRetargeting = false;

        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].nStartTime = 1534490155;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].nTimeout   = 1764490155;   // 2024-12-25 00:00:00

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].nStartTime = 1534490155;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].nTimeout   = 1764490155;   // 2024-12-25 18:00:00

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].nStartTime = 1534490155;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].nTimeout   = 1764490155;   // 2024-12-25 18:00:00

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to LuckyCoin values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1724732207; // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1764490155;   // 2024-12-25 18:00:00

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1724732207; // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1764490155;   // 2024-12-25 18:00:00

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // 4,303,965

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200");

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x2047;
        consensus.fStrictChainId = true;  // we set this to false as block is nonAuxPow
        consensus.nBlockAfterAuxpowRewardThreshold = 5;
        consensus.nAuxpowStartHeight = 20000;

        //consensus.fAllowLegacyBlocks = true;

        // We do not activate digishield in this consensus
        digishieldConsensus = consensus;

        digishieldConsensus.nHeightEffective = 0xFFFFFFFF; // like never

        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 70;

        // Not implementing digishield yet
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = false;

        // Not implementing AuxPow hardfork yet
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();
        //auxpowConsensus.fAllowLegacyBlocks = true;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */

        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xdb;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 8697;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1317529878, 385610221, 0x1e0ffff0, 1, 50 * COIN); // 12097647


        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        assert(consensus.hashGenesisBlock == uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200"));
        assert(genesis.hashMerkleRoot == uint256S("0x40a627262ed716f0f3d5104315fe0b600bf8e32a021929299163f74151fa52b1"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        //vSeeds.push_back(CDNSSeedData("luckycoinfoundation.org", "dnsseed.luckycoinfoundation.org", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,127);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,140);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        /*checkpointData = (CCheckpointData) {
                boost::assign::map_list_of
                        (      0, uint256S("0x1a91e3dace36e2be3bf030a65679fe821aa1d6ef92e7c9902eb318182c355691"))
                        ( 14000, uint256S("0x0f3bec6968ad87171063c28f696f133c0e4bdf3a0bd56d369be658367dd21ee9"))
        };

        chainTxData = ChainTxData{
                // Data as of block ed7d266dcbd8bb8af80f9ccb8deb3e18f9cc3f6972912680feeb37b090f8cee0 (height 4303965).
                // Tx estimate based on average between 2021-07-01 (3793538) and 2022-07-01 (4288126)
                1702700138, // * UNIX timestamp of last checkpoint block
                15423,   // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
                0.1        // * estimated number of transactions per second after checkpoint
        };*/

        checkpointData = (CCheckpointData) {
                boost::assign::map_list_of
                        (      0, uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200"))
        };

        chainTxData = ChainTxData{
                // Data as of block ed7d266dcbd8bb8af80f9ccb8deb3e18f9cc3f6972912680feeb37b090f8cee0 (height 4303965).
                // Tx estimate based on average between 2021-07-01 (3793538) and 2022-07-01 (4288126)
                0, // * UNIX timestamp of last checkpoint block
                0,   // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
                0        // * estimated number of transactions per second after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        consensus.nSubsidyHalvingInterval = 100000;

        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;


        // After deployments are activated we can change it
        consensus.BIP34Hash = uint256S("0x00"); // unused for now.
        consensus.BIP65Height = 99999999;
        consensus.BIP65Height = 99999999;
        consensus.BIP66Height = 99999999;


        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowNoRetargeting = false;

        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].nStartTime = 1703462400;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].nTimeout   = 1735084800;   // 2024-12-25 00:00:00

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].nStartTime = 1703462400;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].nTimeout   = 1735084800;   // 2024-12-25 18:00:00


        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].nStartTime = 1703462400;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].nTimeout   = 1735084800;   // 2024-12-25 18:00:00


        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to LuckyCoin values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1703462400; // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1735084800;   // 2024-12-25 18:00:00

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1703462400; // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1735084800;   // 2024-12-25 18:00:00

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // 4,303,965

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200");


        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x2032; // 98 - Josh Wise!
        consensus.nAuxpowStartHeight = 0; // -1 will always allow legacy blocks
        consensus.nBlockAfterAuxpowRewardThreshold = 5;
        consensus.fStrictChainId = true;
        //consensus.fAllowLegacyBlocks = true;

        // We do not activate digishield in this consensus
        digishieldConsensus = consensus;

        digishieldConsensus.nHeightEffective = 0xFFFFFFFF; // like never

        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Not implementing digishield yet
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;

        // Not implementing AuxPow hardfork yet
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();
       // auxpowConsensus.fAllowLegacyBlocks = true;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;

        nDefaultPort = 18697;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1317529878, 385610221, 0x1e0ffff0, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        assert(consensus.hashGenesisBlock == uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200"));
        assert(genesis.hashMerkleRoot == uint256S("0x40a627262ed716f0f3d5104315fe0b600bf8e32a021929299163f74151fa52b1"));


        // nodes with support for servicebits filtering should be at the top
        //vSeeds.push_back(CDNSSeedData("belscan.io", "testnetseed.belscan.io", true));
        //vSeeds.push_back(CDNSSeedData("belscan.io", "testnetseeder.belscan.io", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,13);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,9);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,141);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0xfa)(0xca)(0xfd).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0xfa)(0xc3)(0x98).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
                boost::assign::map_list_of
                        (      0, uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200"))
        };

        chainTxData = ChainTxData{
                // Data as of block ed7d266dcbd8bb8af80f9ccb8deb3e18f9cc3f6972912680feeb37b090f8cee0 (height 4303965).
                // Tx estimate based on average between 2021-07-01 (3793538) and 2022-07-01 (4288126)
                0, // * UNIX timestamp of last checkpoint block
                0,   // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
                0        // * estimated number of transactions per second after checkpoint
        };
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        // Not used in LuckyCoin
        consensus.nSubsidyHalvingInterval = 100000;

        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;


        // After deployments are activated we can change it
        consensus.BIP34Hash = uint256S("0x00"); // unused for now.
        consensus.BIP65Height = 99999999;
        consensus.BIP65Height = 99999999;
        consensus.BIP66Height = 99999999;


        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowNoRetargeting = false;


        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].nStartTime = 1703462400;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP34].nTimeout   = 1735084800;   // 2024-12-25 00:00:00

        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].nStartTime = 1703462400;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP66].nTimeout   = 1735084800;   // 2024-12-25 18:00:00


        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].nStartTime = 1703462400;   // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP65].nTimeout   = 1735084800;   // 2024-12-25 18:00:00


        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to LuckyCoin values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1703462400; // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1735084800;   // 2024-12-25 18:00:00

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1703462400; // 2023-12-25 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1735084800;   // 2024-12-25 18:00:00

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // 4,303,965

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200");

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x2021; // 98 - Josh Wise!
        consensus.nAuxpowStartHeight = 0; // -1 will always allow legacy blocks
        consensus.nBlockAfterAuxpowRewardThreshold = 5;
        consensus.fStrictChainId = true;

        // We do not activate digishield in this consensus
        digishieldConsensus = consensus;

        digishieldConsensus.nHeightEffective = 0xFFFFFFFF; // like never

        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Not implementing digishield yet
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;

        // Not implementing AuxPow hardfork yet
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();
        //auxpowConsensus.fAllowLegacyBlocks = false;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xc0;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xc0;
        nDefaultPort = 18697;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1317529878, 385610221, 0x1e0ffff0, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        assert(consensus.hashGenesisBlock == uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200"));
        assert(genesis.hashMerkleRoot == uint256S("0x40a627262ed716f0f3d5104315fe0b600bf8e32a021929299163f74151fa52b1"));

        //assert(consensus.hashGenesisBlock == uint256S("0x324635c8e36f663b0adb126a21ad0bd7fa43cc5c5f15aec992bf4dde650bc0ea"));
        //assert(genesis.hashMerkleRoot == uint256S("0x6f80efd038566e1e3eab3e1d38131604d06481e77f2462235c6a9a94b1f8abf9"));

        // nodes with support for servicebits filtering should be at the top
        //vSeeds.push_back(CDNSSeedData("belscan.io", "testnetseed.belscan.io", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,47);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0xfa)(0xca)(0xfd).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0xfa)(0xc3)(0x98).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
                boost::assign::map_list_of
                        ( 0, uint256S("0x002a91713910bc96eb0edf237fcd2799d7a01186e1e96023e860bc70b3916200"))
        };

        chainTxData = ChainTxData{
                0,
                0,
                0
        };
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this -> nHeightEffective && this -> pLeft != NULL) {
        return this -> pLeft -> GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this -> nHeightEffective && this -> pRight != NULL) {
        const Consensus::Params *pCandidate = this -> pRight -> GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
        return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
        return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}