/*
 ***************************************** 
    Como Fua - Beat 'em Up game prototype
    By James Romero. Made with Unity
    2021
 *****************************************
 */

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// All the posible attacking states that the player's character can have, including no attack at all ("NONE" state)
public enum ComboState
{
    NONE,
    PUNCH1,
    PUNCH2,
    PUNCH3,
    KICK1,
    KICK2
}

////////////////////////////////////////////////////////////////////////////// 
//
// This class handles the attack/combo system for the player's character 
//
////////////////////////////////////////////////////////////////////////////// 

public class PlayerAttacks : MonoBehaviour
{
    // Reference to the helper class that handles which animation to play given the player's actions
    private CharacterAnimation player_anim;

    // Players will have only a few seconds to quickly perform a combo
    // So this is to know when a combo is starting for activating the timer
    private bool activate_timer_to_reset;

    // Default time that the player will have to perform a combo (in seconds)
    private float default_combo_timer = 0.4f;

    private float current_combo_timer;

    private ComboState current_combo_state;

    // Awake is called when the script instance is being loaded ///////
    void Awake()
    {
        player_anim = GetComponentInChildren<CharacterAnimation>();
    }
    /////////////////////////////////////////////////////////////////

    // Start is called before the first frame update /////////
    void Start()
    {
        current_combo_timer = default_combo_timer;
        current_combo_state = ComboState.NONE;
    }
    ////////////////////////////////////////////////////////

    // Update is called once per frame /////////////////////////////////
    void Update()
    {
        // If the game isn't paused, handle the combo system given the player's inputs
        if (!PauseControl.game_is_paused)
        {
            ComboAttacks();
            ResetComboState();
        }
    }
    ///////////////////////////////////////////////////////////////////

    // ComboAttacks will process the player's input and establish the attack state that its character will adopt ////
    void ComboAttacks()
    {
        if (Input.GetKeyDown(KeyCode.J)) // For Punching
        {

            if( current_combo_state == ComboState.PUNCH3 ||
                current_combo_state == ComboState.KICK1 ||
                current_combo_state == ComboState.KICK2 )
            {
                return; // To prevent it to try kicking when pressing the button too fast, causing a bug
            }

            current_combo_state++; // Combo state starts at "NONE" and goes to every punch when pressing J repeatedly
            activate_timer_to_reset = true; // Start running the timer to break or continue the combo
            current_combo_timer = default_combo_timer;

            // Play the corresponding punching animation given the current character attack state
            if(current_combo_state == ComboState.PUNCH1)
            {
                player_anim.Punch1();
            }

            if (current_combo_state == ComboState.PUNCH2)
            {
                player_anim.Punch2();
            }

            if (current_combo_state == ComboState.PUNCH3)
            {
                player_anim.Punch3();
            }
        }

        if (Input.GetKeyDown(KeyCode.K)) //For Kicking
        {

            if ( current_combo_state == ComboState.KICK2 ||
                current_combo_state == ComboState.PUNCH3 )
            {
                return; //if current attack is kick2 or punch3 simply return because there's no combo to perform
            }

            if (current_combo_state == ComboState.NONE ||
                current_combo_state == ComboState.PUNCH1 ||
                current_combo_state == ComboState.PUNCH2 )
            {
                current_combo_state = ComboState.KICK1; //chain the kicks when idle, punch1 or punch2
            }
            else if(current_combo_state == ComboState.KICK1)
            {
                current_combo_state++; //If pressing quickly chain the 2nd kick
            }

            activate_timer_to_reset = true; // Start running the timer to break or continue the combo
            current_combo_timer = default_combo_timer;

            // Play the corresponding kicking animation given the current character attack state
            if (current_combo_state == ComboState.KICK1)
            {
                player_anim.Kick1();
            }
            if (current_combo_state == ComboState.KICK2)
            {
                player_anim.Kick2();
            }

        }

    } 
    // Combo Attacks ////////////////////////////////////////////////////////////////////////////////////////

    //Players will have only a few seconds to quickly perform a combo ///////////////////////////////////
    void ResetComboState()
    {
        if (activate_timer_to_reset) //this is only true when we start an attack with a button press
        {
            current_combo_timer -= Time.deltaTime; //time to perform a combo will start running down

            if (current_combo_timer <= 0f) //if we ran out of time
            {
                current_combo_state = ComboState.NONE; //the combo is cancelled

                activate_timer_to_reset = false;
                current_combo_timer = default_combo_timer;
            }
        }
    }// Reset combo state /////////////////////////////////////////////////////////////////////////////

}
