#pragma once

#include "CmD3D9Prerequisites.h"
#include "CmD3D9Resource.h"
#include "CmEventQuery.h"

namespace BansheeEngine
{
	/**
	 * @copydoc EventQuery
	 */
	class CM_D3D9_EXPORT D3D9EventQuery : public EventQuery, public D3D9Resource
	{
	public:
		D3D9EventQuery();
		~D3D9EventQuery();

		/**
		 * @copydoc EventQuery::begin
		 */
		virtual void begin();

		/**
		 * @copydoc EventQuery::isReady
		 */
		virtual bool isReady() const;

		/**
		 * @copydoc	D3D9Resource::notifyOnDeviceCreate
		 */
		virtual void notifyOnDeviceCreate(IDirect3DDevice9* d3d9Device);

		/**
		 * @copydoc	D3D9Resource::notifyOnDeviceDestroy
		 */
		virtual void notifyOnDeviceDestroy(IDirect3DDevice9* d3d9Device);

		/**
		 * @copydoc	D3D9Resource::notifyOnDeviceLost
		 */
		virtual void notifyOnDeviceLost(IDirect3DDevice9* d3d9Device);

		/**
		 * @copydoc	D3D9Resource::notifyOnDeviceReset
		 */
		virtual void notifyOnDeviceReset(IDirect3DDevice9* d3d9Device);

	private:
		/**
		 * @brief	Creates the internal DX9 query.
		 */
		void createQuery();

		/**
		 * @brief	Releases the internal DX9 query.
		 */
		void releaseQuery();

	private:
		bool mQueryIssued;
		IDirect3DQuery9* mQuery;
		IDirect3DDevice9* mDevice;
	};
}