/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

//app
#include "proportional_splitter.hpp"

BEGIN_EVENT_TABLE (ProportionalSplitterWindow, wxSplitterWindow)
  EVT_SIZE (ProportionalSplitterWindow::OnSize)
END_EVENT_TABLE ()

ProportionalSplitterWindow::ProportionalSplitterWindow (
  float proportion, wxWindow * parent, wxWindowID id, 
  const wxPoint & pos, const wxSize & size, long style,
  const wxString & name)
  : wxSplitterWindow (parent, id, pos, size, style, name),
    m_proportion (proportion)
{
}

bool
ProportionalSplitterWindow::SplitVertically (wxWindow * window1,
                                             wxWindow * window2,
                                             int sashPosition)
{
  bool result =
    wxSplitterWindow::SplitVertically (window1, window2, sashPosition);
  m_old_length_tot = GetLength (GetSize ());
  return result;
}

bool
ProportionalSplitterWindow::SplitHorizontally (wxWindow * window1,
                                               wxWindow * window2,
                                               int sashPosition)
{
  bool result =
    wxSplitterWindow::SplitHorizontally (window1, window2, sashPosition);
  m_old_length_tot = GetLength (GetSize ());
  return result;
}

void
ProportionalSplitterWindow::OnSize (wxSizeEvent & event)
{
  // Require both windows present to do adjustments
  if (GetWindow1 () && GetWindow2 ())
  {
    int new_length_tot, dead_space;
    float new_length[2];

    // Get new lengths
    new_length_tot = GetLength (event.m_size);
    new_length[0] = GetLength (GetWindow1 ()->GetSize ());
    new_length[1] = GetLength (GetWindow2 ()->GetSize ());
    dead_space = GetSashSize () + GetBorderSize () * 2;

    // If there's not enough space for both to be bigger than the minimum pane
    // size, we have to resort to distributing the remaining space evenly,
    // skipping the proportions
    float maximum_potential_size = new_length_tot - dead_space;
    if (maximum_potential_size < GetMinimumPaneSize () * 2)
    {
      new_length[0] = maximum_potential_size / 2.0f;
      new_length[1] = new_length_tot - new_length[0] - dead_space;
    }
    else
    {
      // Distribute according to proportions
      // Note that proportion = 0.0 equals standard splitter behaviour, that is:
      // always leaves the splitter in place
      new_length[0] += (new_length_tot - m_old_length_tot) * m_proportion;
      new_length[1] = new_length_tot - new_length[0] - dead_space;

      // If this results in less than legal size for any of the windows, we have
      // to lock it to that size, adjusting the other one to fit
      // We know this is going to work, because we've made certain we've got
      // enough space for at least two minimum-sized panes
      for (int i = 0; i < 2; i++)
        if (new_length[i] < GetMinimumPaneSize ())
        {
          new_length[1 - i] -= GetMinimumPaneSize () - new_length[i];
          new_length[i] = GetMinimumPaneSize ();
          break;
        }
    }

    m_old_length_tot = new_length_tot;

    // Make the nescessary adjustments
    SetSashPosition (new_length[0] - GetBorderSize ());
  }
}

int
ProportionalSplitterWindow::GetLength (const wxSize & size) const
{
  return (GetSplitMode () ==
          wxSPLIT_HORIZONTAL ? size.GetHeight () : size.GetWidth ());
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
