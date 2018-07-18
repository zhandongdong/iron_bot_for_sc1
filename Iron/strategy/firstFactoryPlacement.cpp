//////////////////////////////////////////////////////////////////////////
//
// This file is part of Iron's source files.
// Iron is free software, licensed under the MIT/X11 License. 
// A copy of the license is provided with the library in the LICENSE file.
// Copyright (c) 2016, 2017, Igor Dimitrijevic
//
//////////////////////////////////////////////////////////////////////////


#include "firstFactoryPlacement.h"
#include "strategy.h"
#include "cannonRush.h"
#include "workerRush.h"
#include "zerglingRush.h"
#include "stone.h"
#include "../units/cc.h"
#include "../behavior/fleeing.h"
#include "../behavior/constructing.h"
#include "../behavior/walking.h"
#include "../behavior/defaultBehavior.h"
#include "../Iron.h"

namespace { auto & bw = Broodwar; }




namespace iron
{


//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                  class FirstFactoryPlacement
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////


FirstFactoryPlacement::FirstFactoryPlacement()
{
}


FirstFactoryPlacement::~FirstFactoryPlacement()
{
	if (m_pBuilder)
		if (m_pBuilder->GetBehavior()->IsWalking())
			m_pBuilder->ChangeBehavior<DefaultBehavior>(m_pBuilder);
}


string FirstFactoryPlacement::StateDescription() const
{
	if (!m_active) return "-";
	if (m_active) return "active";

	return "-";
}


void FirstFactoryPlacement::OnBWAPIUnitDestroyed(BWAPIUnit * pBWAPIUnit)
{
	if (m_pBuilder == pBWAPIUnit)
		m_pBuilder = nullptr;
}



void FirstFactoryPlacement::OnFrame_v()
{
	if (ai()->GetStrategy()->Active<Stone>())
		return Discard();

	if (him().IsProtoss()) return Discard();
//	if (him().IsZerg()) return Discard();
	if (!me().Buildings(Terran_Factory).empty()) return Discard();
	if (ai()->GetStrategy()->Detected<CannonRush>()) return Discard();
	
	if (ai()->GetStrategy()->Detected<WorkerRush>()) return Discard();
	if (ai()->GetStrategy()->Detected<ZerglingRush>()) return Discard();

	if (m_active)
	{
		if (m_pBuilder)
		{
			if (!(m_pBuilder->GetBehavior()->IsWalking() ||
				m_pBuilder->GetBehavior()->IsConstructing() && (m_pBuilder->GetBehavior()->IsConstructing()->Location() == m_location)))
				// m_pBuilder may be fleeing
				return Discard();

			// Walking agents do not check for threats themselves.
			if (m_pBuilder->GetBehavior()->IsWalking())
				if (!findThreats(m_pBuilder, 3*32).empty())
					return m_pBuilder->ChangeBehavior<Fleeing>(m_pBuilder);
		}
		else
		{
			if (My<Terran_SCV> * pWorker = findFreeWorker(me().StartingVBase()))
			{
				for (;;)
				{
					int distance = roundedDist(center(m_location), pWorker->Pos());

					if (My<Terran_SCV> * pCloserWorker = findFreeWorker(me().StartingVBase(), [this, distance](const My<Terran_SCV> * pSCV)
						{ return roundedDist(center(m_location), pSCV->Pos()) < distance - 5; }))
						pWorker = pCloserWorker;
					else break;
				} 


				pWorker->ChangeBehavior<Walking>(pWorker, Position(m_location + UnitType(Terran_Factory).tileSize()/2), __FILE__ + to_string(__LINE__));
				m_pBuilder = pWorker;
				return;
			}
		}
	}
	else
	{
		if (ai()->Me().GetVArea()->HotCP())
			if (me().SupplyUsed() >= 16)
			{
				if (m_location == TilePositions::None) m_location = Constructing::FindFirstFactoryPlacement();
				if (m_location == TilePositions::None) return;

				int distance = roundedDist(m_location, TilePosition(me().StartingBase()->Center()));
				if ((ai()->Me().MineralsAvailable() > 200-4*distance) && (ai()->Me().GasAvailable() > 100-2*distance))
				{
				///	ai()->SetDelay(100);
					m_active = true;
					m_activeSince = ai()->Frame();
					return;
				}
			}
	}
}


} // namespace iron



