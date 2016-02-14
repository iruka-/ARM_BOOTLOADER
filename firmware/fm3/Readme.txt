==========================================================================
              Sample Driver Software (USB Function) Project  
==========================================================================
                  (C)2011 FUJITSU SEMICONDUCTOR LIMITED                
                                                                        
   The following software deliverable is intended for and must only be  
   used for reference and in an evaluation laboratory environment.      
   It is provided on an as-is basis without charge and is subject to    
   alterations.                                                         
   It is the user's obligation to fully test the software in its        
   environment and to ensure proper functionality, qualification and    
   compliance with component specifications.                            
                                                                        
   In the event the software deliverable includes the use of open       
   source components, the provisions of the governing open source       
   license agreement shall apply with respect to such software          
   deliverable.                                                         
   FSL does not warrant that the deliverables do not infringe any       
   third party intellectual property right (IPR). In the event that     
   the deliverables infringe a third party IPR it is the sole           
   responsibility of the customer to obtain necessary licenses to       
   continue the usage of the deliverable.                               
                                                                        
   To the maximum extent permitted by applicable law FSL disclaims all  
   warranties, whether express or implied, in particular, but not       
   limited to, warranties of merchantability and fitness for a          
   particular purpose for which the deliverable is not designated.      
                                                                        
   To the maximum extent permitted by applicable law, FSL's liability   
   is restricted to intention and gross negligence.                     
   FSL is not liable for consequential damages.                         

==========================================================================

History
Date          Ver    Description
2011-02-18    1.0    initial version
2011-05-24    1.1    Remove core_cm*.* in common folder
==========================================================================

This is a USB function project sample for the MB9BF506R. It includes some
basic settings for e.g. Linker, C-Compiler.

Clock settings:
---------------
Crystal:  4 MHz
HCLK:    64 MHz
PCLK0:   32 MHz
PCLK1:   32 MHz
PCLK2:   32 MHz
