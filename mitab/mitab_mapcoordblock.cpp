/**********************************************************************
 * $Id: mitab_mapcoordblock.cpp,v 1.1 1999-07-12 04:18:24 daniel Exp $
 *
 * Name:     mitab_mapcoordblock.cpp
 * Project:  MapInfo TAB Read/Write library
 * Language: C++
 * Purpose:  Implementation of the TABMAPCoordBlock class used to handle
 *           reading/writing of the .MAP files' coordinate blocks
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
 * $Log: mitab_mapcoordblock.cpp,v $
 * Revision 1.1  1999-07-12 04:18:24  daniel
 * Initial checkin
 *
 **********************************************************************/

#include "mitab.h"

/*=====================================================================
 *                      class TABMAPCoordBlock
 *====================================================================*/

#define MAP_COORD_HEADER_SIZE   8

/**********************************************************************
 *                   TABMAPCoordBlock::TABMAPCoordBlock()
 *
 * Constructor.
 **********************************************************************/
TABMAPCoordBlock::TABMAPCoordBlock():
    TABRawBinBlock(TRUE)
{
    m_nCenterX = m_nCenterY = m_nNextCoordBlock = m_numDataBytes = 0;
}

/**********************************************************************
 *                   TABMAPCoordBlock::~TABMAPCoordBlock()
 *
 * Destructor.
 **********************************************************************/
TABMAPCoordBlock::~TABMAPCoordBlock()
{

}


/**********************************************************************
 *                   TABMAPCoordBlock::InitBlockData()
 *
 * Perform some initialization on the block after its binary data has
 * been set or changed (or loaded from a file).
 *
 * Returns 0 if succesful or -1 if an error happened, in which case 
 * CPLError() will have been called.
 **********************************************************************/
int     TABMAPCoordBlock::InitBlockData(GByte *pabyBuf, int nSize, 
                                         GBool bMakeCopy /* = TRUE */,
                                         FILE *fpSrc /* = NULL */, 
                                         int nOffset /* = 0 */)
{
    int nStatus;

    /*-----------------------------------------------------------------
     * First of all, we must call the base class' InitBlockData()
     *----------------------------------------------------------------*/
    nStatus = TABRawBinBlock::InitBlockData(pabyBuf, nSize, bMakeCopy,
                                            fpSrc, nOffset);
    if (nStatus != 0)   
        return nStatus;

    /*-----------------------------------------------------------------
     * Validate block type
     *----------------------------------------------------------------*/
    if (m_nBlockType != TABMAP_COORD_BLOCK)
    {
        CPLError(CE_Failure, CPLE_FileIO,
                 "InitBlockData(): Invalid Block Type: got %d expected %d",
                 m_nBlockType, TABMAP_COORD_BLOCK);
        CPLFree(m_pabyBuf);
        m_pabyBuf = NULL;
        return -1;
    }

    /*-----------------------------------------------------------------
     * Init member variables
     *----------------------------------------------------------------*/
    GotoByteInBlock(0x002);
    m_numDataBytes = ReadInt16();       /* Excluding 8 bytes header */

    m_nNextCoordBlock = ReadInt32();

    /*-----------------------------------------------------------------
     * The read ptr is now located at the beginning of the data part.
     *----------------------------------------------------------------*/
    GotoByteInBlock(MAP_COORD_HEADER_SIZE);

    return 0;
}

/**********************************************************************
 *                   TABMAPObjectBlock::SetComprCoordOrigin()
 *
 * Set the Compressed integer coordinates space origin to be used when
 * reading compressed coordinates using ReadIntCoord().
 **********************************************************************/
void     TABMAPCoordBlock::SetComprCoordOrigin(GInt32 nX, GInt32 nY)
{
    m_nCenterX = nX;
    m_nCenterY = nY;
}

/**********************************************************************
 *                   TABMAPObjectBlock::ReadIntCoord()
 *
 * Read the next pair of integer coordinates value from the block, and
 * apply the translation relative to the origin of the coord. space
 * previously set using SetComprCoordOrigin() if bCompressed=TRUE.
 *
 * This means that the returned coordinates are always absolute integer
 * coordinates, even when the source coords are in compressed form.
 *
 * Returns 0 if succesful or -1 if an error happened, in which case 
 * CPLError() will have been called.
 **********************************************************************/
int     TABMAPCoordBlock::ReadIntCoord(GBool bCompressed, 
                                        GInt32 &nX, GInt32 &nY)
{
    if (bCompressed)
    {   
        nX = m_nCenterX + ReadInt16();
        nY = m_nCenterY + ReadInt16();
    }
    else
    {
        nX = ReadInt32();
        nY = ReadInt32();
    }

    if (CPLGetLastErrorNo() != 0)
        return -1;

    return 0;
}

/**********************************************************************
 *                   TABMAPObjectBlock::ReadIntCoords()
 *
 * Read the specified number of pairs of X,Y integer coordinates values
 * from the block, and apply the translation relative to the origin of
 * the coord. space previously set using SetComprCoordOrigin() if 
 * bCompressed=TRUE.
 *
 * This means that the returned coordinates are always absolute integer
 * coordinates, even when the source coords are in compressed form.
 *
 * panXY should point to an array big enough to receive the specified
 * number of coordinates.
 *
 * Returns 0 if succesful or -1 if an error happened, in which case 
 * CPLError() will have been called.
 **********************************************************************/
int     TABMAPCoordBlock::ReadIntCoords(GBool bCompressed, int numCoordPairs, 
                                        GInt32 *panXY)
{
    int i, numValues = numCoordPairs*2;

    if (bCompressed)
    {   
        for(i=0; i<numValues; i+=2)
        {
            panXY[i]   = m_nCenterX + ReadInt16();
            panXY[i+1] = m_nCenterY + ReadInt16();
            if (CPLGetLastErrorNo() != 0)
                return -1;
        }
    }
    else
    {
        for(i=0; i<numValues; i+=2)
        {
            panXY[i]   = ReadInt32();
            panXY[i+1] = ReadInt32();
            if (CPLGetLastErrorNo() != 0)
                return -1;
        }
    }

    return 0;
}

/**********************************************************************
 *                   TABMAPObjectBlock::ReadCoordSecHdrs()
 *
 * Read a set of coordinate section headers for PLINE MULTIPLE or REGIONs
 * and store the result in the array of structures pasHdrs[].  It is assumed
 * that pasHdrs points to an allocated array of at least numSections 
 * TABMAPCoordSecHdr structures.
 *
 * The function will also set the values of numVerticesTotal to the 
 * total number of coordinates in the object (the sum of all sections 
 * headers read).
 *
 * At the end of the call, this TABMAPCoordBlock object will be located
 * at the beginning of the coordinate data.
 *
 * IMPORTANT: This function makes the assumption that coordinates for all
 *            the sections are grouped together immediately after the
 *            last section header block (i.e. that the coord. data is not
 *            located all over the place).  If it is not the case then
 *            an error will be produced and the code to read region and
 *            multipline objects will have to be updated. 
 *
 * Returns 0 if succesful or -1 if an error happened, in which case 
 * CPLError() will have been called.
 **********************************************************************/
int     TABMAPCoordBlock::ReadCoordSecHdrs(GBool bCompressed, int numSections,
                                           TABMAPCoordSecHdr *pasHdrs,
                                           int    &numVerticesTotal)
{
    int i, nTotalHdrSize;

    if (bCompressed)
        nTotalHdrSize = 16 * numSections;
    else
        nTotalHdrSize = 24 * numSections;

    numVerticesTotal = 0;

    for(i=0; i<numSections; i++)
    {
        /*-------------------------------------------------------------
         * Read the coord. section header blocks
         *------------------------------------------------------------*/
        pasHdrs[i].numVertices = ReadInt16();
        pasHdrs[i].numHoles = ReadInt16();
        ReadIntCoord(bCompressed, pasHdrs[i].nXMin, pasHdrs[i].nYMin);
        ReadIntCoord(bCompressed, pasHdrs[i].nXMax, pasHdrs[i].nYMax);
        pasHdrs[i].nDataOffset = ReadInt32();

        if (CPLGetLastErrorNo() != 0)
            return -1;

        numVerticesTotal += pasHdrs[i].numVertices;
        pasHdrs[i].nVertexOffset = (pasHdrs[i].nDataOffset - nTotalHdrSize)/8;

    }

    for(i=0; i<numSections; i++)
    {
        /*-------------------------------------------------------------
         * Make sure all coordinates are grouped together
         * (Well... at least check that all the vertex indices are enclosed
         * inside the [0..numVerticesTotal] range.)
         *------------------------------------------------------------*/
        if ( pasHdrs[i].nVertexOffset < 0 || 
             (pasHdrs[i].nVertexOffset +
                           pasHdrs[i].numVertices ) > numVerticesTotal)
        {
            CPLError(CE_Failure, CPLE_AssertionFailed,
                     "Unsupported case or corrupt file: MULTIPLINE/REGION "
                     "object vertices do not appear to be grouped together.");
            return -1;
        }
    }

    return 0;
}

/**********************************************************************
 *                   TABMAPCoordBlock::ReadBytes()
 *
 * Cover function for TABRawBinBlock::ReadBytes() that will automagically
 * load the next coordinate block in the chain before reading the 
 * requested bytes if we are at the end of the current block and if
 * m_nNextCoordBlock is a valid block.
 *
 * Then the control is passed to TABRawBinBlock::ReadBytes() to finish the
 * work:
 * Copy the number of bytes from the data block's internal buffer to
 * the user's buffer pointed by pabyDstBuf.
 *
 * Passing pabyDstBuf = NULL will only move the read pointer by the
 * specified number of bytes as if the copy had happened... but it 
 * won't crash.
 *
 * Returns 0 if succesful or -1 if an error happened, in which case 
 * CPLError() will have been called.
 **********************************************************************/
int     TABMAPCoordBlock::ReadBytes(int numBytes, GByte *pabyDstBuf)
{
    int nStatus;

    if (m_pabyBuf && 
        m_nCurPos >= (m_numDataBytes+MAP_COORD_HEADER_SIZE) && 
        m_nNextCoordBlock > 0)
    {
        if ( (nStatus=GotoByteInFile(m_nNextCoordBlock)) != 0)
        {
            // Failed.... an error has already been reported.
            return nStatus;
        }

        GotoByteInBlock(8);      // Move pointer past header
    }

    return TABRawBinBlock::ReadBytes(numBytes, pabyDstBuf);
}

/**********************************************************************
 *                   TABMAPCoordBlock::Dump()
 *
 * Dump block contents... available only in DEBUG mode.
 **********************************************************************/
#ifdef DEBUG

void TABMAPCoordBlock::Dump(FILE *fpOut /*=NULL*/)
{
    if (fpOut == NULL)
        fpOut = stdout;

    fprintf(fpOut, "----- TABMAPCoordBlock::Dump() -----\n");
    if (m_pabyBuf == NULL)
    {
        fprintf(fpOut, "Block has not been initialized yet.");
    }
    else
    {
        fprintf(fpOut,"Coordinate Block (type %d) at offset %d.\n", 
                                                 m_nBlockType, m_nFileOffset);
        fprintf(fpOut,"  m_numDataBytes        = %d\n", m_numDataBytes);
        fprintf(fpOut,"  m_nNextCoordBlock     = %d\n", m_nNextCoordBlock);
    }

    fflush(fpOut);
}

#endif // DEBUG



