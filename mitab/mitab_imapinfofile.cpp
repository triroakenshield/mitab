/**********************************************************************
 * $Id: mitab_imapinfofile.cpp,v 1.2 1999-11-08 19:15:44 stephane Exp $
 *
 * Name:     mitab_imapinfo
 * Project:  MapInfo mid/mif Tab Read/Write library
 * Language: C++
 * Purpose:  Implementation of the IMapInfoFile class, super class of
 *           of MIFFile and TABFile
 * Author:   Daniel Morissette, danmo@videotron.ca
 *
 **********************************************************************
 * Copyright (c) 1999, Daniel Morissette
 *
 * All rights reserved.  This software may be copied or reproduced, in
 * all or in part, without the prior written consent of its author,
 * Daniel Morissette (danmo@videotron.ca).  However, any material copied
 * or reproduced must bear the original copyright notice (above), this 
 * original paragraph, and the original disclaimer (below).
 * 
 * The entire risk as to the results and performance of the software,
 * supporting text and other information contained in this file
 * (collectively called the "Software") is with the user.  Although 
 * considerable efforts have been used in preparing the Software, the 
 * author does not warrant the accuracy or completeness of the Software.
 * In no event will the author be liable for damages, including loss of
 * profits or consequential damages, arising out of the use of the 
 * Software.
 * 
 **********************************************************************
 *
 * $Log: mitab_imapinfofile.cpp,v $
 * Revision 1.2  1999-11-08 19:15:44  stephane
 * Add headers method
 *
 * Revision 1.1  1999/11/08 04:17:27  stephane
 * First Revision
 *
 *
 **********************************************************************/

#include "mitab.h"


/**********************************************************************
 *                   IMapInfoFile::IMapInfoFile()
 *
 * Constructor.
 **********************************************************************/
IMapInfoFile::IMapInfoFile()
{
    m_poFilterGeom = NULL;    
}


/**********************************************************************
 *                   IMapInfoFile::~IMapInfoFile()
 *
 * Destructor.
 **********************************************************************/
IMapInfoFile::~IMapInfoFile()
{
    if( m_poFilterGeom != NULL )
    {
	delete m_poFilterGeom;
	m_poFilterGeom = NULL;
    }
}

/**********************************************************************
 *                   IMapInfoFile::GetNextFeature()
 *
 * Standard OGR GetNextFeature implementation.  This methode is used
 * to retreive the next OGRFeature.
 **********************************************************************/
OGRFeature *IMapInfoFile::GetNextFeature()
{
    OGRFeature *poFeature, *poFeatureRef;
      
    poFeatureRef = GetFeatureRef(m_nCurFeatureId+1);
    if (poFeatureRef)
    {
	poFeature = poFeatureRef->Clone();
	poFeature->SetFID(poFeatureRef->GetFID());
	return poFeature;
    }
    else
      return NULL;
}

/**********************************************************************
 *                   IMapInfoFile::CreateFeature()
 *
 * Standard OGR CreateFeature implementation.  This methode is used
 * to create a new feature in current dataset 
 **********************************************************************/
OGRErr     IMapInfoFile::CreateFeature(OGRFeature *poFeature)
{
    TABFeature *poTABFeature;
    OGRGeometry   *poGeom;

    poGeom = poFeature->GetGeometryRef();

    switch (poGeom->getGeometryType())
    {
      case wkbPoint:
	poTABFeature = new TABPoint(poFeature->GetDefnRef());
	break;
      case wkbPolygon:
	poTABFeature = new TABRegion(poFeature->GetDefnRef());
	break;
      case wkbLineString:
      case wkbMultiPoint:
      case wkbMultiLineString:
      case wkbMultiPolygon:
	poTABFeature = new TABPolyline(poFeature->GetDefnRef());
	break;
      case wkbGeometryCollection:
      case wkbUnknown:
      default:
         poTABFeature = new TABFeature(poFeature->GetDefnRef()); 
        break;
    }

    poTABFeature->SetGeometryDirectly(poGeom->clone());
    
    for (int i=0; i< poFeature->GetDefnRef()->GetFieldCount();i++)
    {
	poTABFeature->SetField(i,poFeature->GetRawFieldRef( i ));
    }
    

    if (SetFeature(poTABFeature) == 0)
      return OGRERR_NONE;
    else
      return OGRERR_FAILURE;
}

/**********************************************************************
 *                   IMapInfoFile::GetFeature()
 *
 * Standard OGR GetFeature implementation.  This methode is used
 * to get the wanted (nFeatureId) feature, a NULL value will be 
 * returned on error.
 **********************************************************************/
OGRFeature *IMapInfoFile::GetFeature(long nFeatureId)
{
    OGRFeature *poFeature, *poFeatureRef;

    
    poFeatureRef = GetFeatureRef(nFeatureId);
    if (poFeatureRef)
    {
	poFeature = poFeatureRef->Clone();
	poFeature->SetFID(poFeatureRef->GetFID());
	
	return poFeature;
    }
    else
      return NULL;
}

/**********************************************************************
 *                   IMapInfoFile::GetSpatialFilter()
 *
 * Standard OGR GetSpatialFilter implementation.  This methode is used
 * to retreive the SpacialFilter object.
 **********************************************************************/
OGRGeometry *IMapInfoFile::GetSpatialFilter()
{
    return m_poFilterGeom;
}


/**********************************************************************
 *                   IMapInfoFile::SetSpatialFilter()
 *
 * Standard OGR SetSpatialFiltere implementation.  This methode is used
 * to set a SpatialFilter for this OGRLayer
 **********************************************************************/
void IMapInfoFile::SetSpatialFilter (OGRGeometry * poGeomIn )

{
    if( m_poFilterGeom != NULL )
    {
        delete m_poFilterGeom;
        m_poFilterGeom = NULL;
    }

    if( poGeomIn != NULL )
        m_poFilterGeom = poGeomIn->clone();
}


