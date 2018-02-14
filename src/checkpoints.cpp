// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    
    
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, uint256("0x4b0d7f133c5267d715d4d8992635a5490d1edd6b7072cce3f8fe116aba983b6a") )
        (77900, uint256("0x907be67958dcd6d9d06c2c896f3b65aad687867ff342db2c7cb0ff5d717c5255"))
        (77988, uint256("0xd87412238aad0d2e53fd6ba6fcc3e429257ee0fc93872ca4716c976caa40a14d"))
        (78000, uint256("0xf535a817b179afce21ead0e5d30940bb5cc76dfa89bb524c57162a45fdd0916c"))
        (78690, uint256("0x05a0a3e7abba6d37a529897e199a9c295357e4645d7c080c0580099e8a6b3c98"))
        (78691, uint256("0x89c9542eecf041961fb4cb083c2652dbb03150be080334c3977bd1a171ace2b5"))
        (78961, uint256("0xdc8da75ee5b362cc8ee55e466c1cd58faf7849f02b28bd2cc68fe9891758c41e"))
        (81686, uint256("0x9c97add390392589888f686c4296062f140ad1d601c92f61ca3607684f9dc8da"))
        (81786, uint256("0xc003a4193f8c83729676eaea3bb8a7fb97d8b208309970e919155ac0c722eef8"))
        (90000, uint256("0xb7a69eb99067eb23cbe1f22f2240ddd45930ab02fda291e6c718d21fc7c58226"))
        (95000, uint256("0x52ec943ff2ae9552308e2f7a3ef5aaddb8d6b7e1d45ada2e607ff60bf6380fb9"))
        (95150, uint256("0x9b9dba004fa65750105b505569db97b635095a9dd0e620729f7fe4103f5576bd"))
        (95350, uint256("0x095f1cb3cf1f1300ad99f891c2c0bb13cc374d9215781ad988e82cc0086a8e45"))
    ;
   
    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
